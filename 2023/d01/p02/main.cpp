#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

template<std::ranges::range Range>
std::vector<std::pair<std::size_t, int>> find_substrs_in(std::string_view value, Range const& substrs)
{
    auto const substr_match = [&value](std::pair<std::string_view, int> const& word)
    {
        std::vector<std::pair<std::size_t, int>> positions;

        std::size_t pos{value.find(word.first, 0)};
        while (pos != std::string::npos)
        {
            positions.emplace_back(pos, word.second);
            pos = value.find(word.first, pos + 1);
        }

        return positions;
    };

    std::vector<std::pair<std::size_t, int>> rv;
    for (auto matches : substrs | std::views::transform(substr_match))
    {
        std::ranges::move(matches, std::back_inserter(rv));
    }
    std::ranges::sort(rv);
    return rv;
}

int main([[maybe_unused]] int argc, char** argv)
{
    using namespace std::string_view_literals;

    std::ifstream input{argv[1]};

    auto const words = {std::make_pair("one"sv, 1),
        std::make_pair("two"sv, 2),
        std::make_pair("three"sv, 3),
        std::make_pair("four"sv, 4),
        std::make_pair("five"sv, 5),
        std::make_pair("six"sv, 6),
        std::make_pair("seven"sv, 7),
        std::make_pair("eight"sv, 8),
        std::make_pair("nine"sv, 9)};

    auto const digits = {std::make_pair("1"sv, 1),
        std::make_pair("2"sv, 2),
        std::make_pair("3"sv, 3),
        std::make_pair("4"sv, 4),
        std::make_pair("5"sv, 5),
        std::make_pair("6"sv, 6),
        std::make_pair("7"sv, 7),
        std::make_pair("8"sv, 8),
        std::make_pair("9"sv, 9)};

    int calibration_sum{};
    std::string line;
    while (getline(input, line))
    {
        std::vector<std::pair<std::size_t, int>> word_match{find_substrs_in(line, words)};
        std::vector<std::pair<std::size_t, int>> digit_match{find_substrs_in(line, digits)};
        std::vector<std::pair<std::size_t, int>> all_matches;
        std::merge(word_match.cbegin(),
            word_match.cend(),
            digit_match.cbegin(),
            digit_match.cend(),
            std::back_inserter(all_matches));

        calibration_sum += all_matches.front().second * 10 + all_matches.back().second;
    }
    std::cout << calibration_sum << '\n';
}
