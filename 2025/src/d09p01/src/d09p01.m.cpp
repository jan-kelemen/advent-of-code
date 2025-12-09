#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    static std::regex const points_regex{R"((\d+),(\d+))"};

    std::vector<std::pair<int, int>> positions;

    std::string line;
    while (std::getline(input, line))
    {
        std::smatch match;
        [[maybe_unused]] bool const matched{
            std::regex_match(line.cbegin(), line.cend(), match, points_regex)};
        assert(matched);

        positions.emplace_back(std::stoi(match[1]), std::stoi(match[2]));
    }

    int64_t area{0};
    for (size_t i{}; i != positions.size() - 1; ++i)
    {
        for (size_t j{i + 1}; j != positions.size(); ++j)
        {
            int64_t const x{
                std::abs(positions[i].first - positions[j].first) + 1};
            int64_t const y{
                std::abs(positions[i].second - positions[j].second) + 1};

            area = std::max(area, x * y);
        }
    }

    fmt::println("Max: {}", area);
}
