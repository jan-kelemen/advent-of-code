#include <fmt/format.h>

#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    std::regex const number_regex{R"((\d+))"};

    std::vector<std::string> lines;

    std::string line;
    while (std::getline(input, line))
    {
        lines.push_back(std::move(line));
    }

    uint64_t total{};
    std::vector<uint64_t> numbers;
    for (int64_t i = lines[0].size() - 1; i >= 0; --i)
    {
        uint64_t intermediate{};
        for (size_t j{}; j != lines.size() - 1; ++j)
        {
            if (lines[j][i] == ' ')
                continue;

            intermediate *= 10;
            intermediate += lines[j][i] - '0';
        }
        numbers.push_back(intermediate);

        if (lines.back()[i] == '+')
        {
            total += std::accumulate(numbers.cbegin(),
                numbers.cend(),
                uint64_t{0},
                std::plus{});
            --i;
            numbers.clear();
        }
        else if (lines.back()[i] == '*')
        {
            total += std::accumulate(numbers.cbegin(),
                numbers.cend(),
                uint64_t{1},
                std::multiplies{});
            --i;
            numbers.clear();
        }
    }

    fmt::println("Total: {}", total);
}
