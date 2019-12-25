#include <numeric>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

int main()
{
  std::ifstream stream{ "input.txt" };
  std::vector<int64_t> fuel_per_mass;
  std::transform(std::istream_iterator<int64_t>{ stream }, {}, std::back_inserter(fuel_per_mass), [](int64_t const v) { return v / 3 - 2; });
  for (int64_t& v : fuel_per_mass)
  {
    int64_t total = v;
    int64_t current = v;
    while ((current / 3 - 2) > 0) { total += current = current / 3 - 2; }
    v = total;
  }
  std::cout << std::accumulate(fuel_per_mass.begin(), fuel_per_mass.end(), int64_t{ 0 });
}