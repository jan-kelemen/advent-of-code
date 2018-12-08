#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <map>
#include <string>

template<typename InputIt>
[[nodiscard]] std::string remove_type(char const type, InputIt begin, InputIt end)
{
    static_assert(std::is_same_v<std::remove_cv_t<typename std::iterator_traits<InputIt>::value_type>, char>);

    using namespace std::string_literals;
    auto polymers = ""s;
    auto last_char = '\0';
    for (; begin != end; ++begin)
    {
        char const current_char = *begin;
        if(std::tolower(current_char) == std::tolower(type))
            continue;

        if (std::tolower(last_char) != std::tolower(current_char) || last_char == current_char)
        {
            last_char = current_char;
            polymers.push_back(last_char);
            continue;
        }

        polymers.pop_back();
        last_char = polymers.empty() ? '\0' : *polymers.rbegin();
    }

    return polymers;
}

int main()
{
    using namespace  std::string_literals;
    auto polymer = ""s;
    std::cin >> polymer;

    auto reacted_length = std::map<char, std::size_t, std::less<>>{};
    for(char const& unit : polymer)
    {
        if(reacted_length.count(unit))
            continue;
        reacted_length[unit] = remove_type(unit, polymer.cbegin(), polymer.cend()).size();
    }
    int const shortest = std::min_element(
        reacted_length.cbegin(), reacted_length.cend(),
        [](std::pair<char, std::size_t> const& l, std::pair<char, std::size_t> const& r) { return l.second < r.second; })->second;

    std::cout << shortest << '\n';
}
