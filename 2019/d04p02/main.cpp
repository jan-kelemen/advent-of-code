#include <iostream>
#include <string>
#include <algorithm>

int main()
{
  std::int64_t count = 0;
  for (int i = 108457; i <= 562041; ++i)
  {
    std::string const value = std::to_string(i);
    bool const has_doubles = std::adjacent_find(value.cbegin(), value.cend()) != value.cend();
    bool const monotonically_increasing = std::is_sorted(value.cbegin(), value.cend());

    if (!(has_doubles && monotonically_increasing)) { continue; }

    bool has_pair_only = std::any_of(
      value.cbegin(), value.cend(),
      [&value](char const c) { return std::count(value.cbegin(), value.cend(), c) == 2; });

    count += static_cast<std::int64_t>(has_pair_only);
  }
  std::cout << count << '\n';
}