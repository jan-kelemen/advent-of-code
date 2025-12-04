#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    std::vector<std::vector<bool>> rolls;

    std::string line;
    while (std::getline(input, line))
    {
        std::ranges::transform(line,
            std::back_inserter(rolls.emplace_back()),
            [](char c) { return c == '@'; });
    }
    auto const test{[&rolls](int const i, int const j)
        {
            if (i < 0 || i >= rolls.size())
                return 0;
            if (j < 0 || j >= rolls[i].size())
                return 0;

            return static_cast<int>(static_cast<bool>(rolls[i][j]));
        }};

    uint64_t accessible{};
    for (int i{}; i != rolls.size(); ++i)
    {
        for (int j{}; j != rolls[i].size(); ++j)
        {
            if (!rolls[i][j])
                continue;

            int surrounding{-1};
            for (int const x : std::views::iota(-1, 2))
            {
                for (int const y : std::views::iota(-1, 2))
                {
                    surrounding += test(i + x, j + y);
                }
            }

            accessible += static_cast<uint64_t>(surrounding < 4);
        }
    }

    fmt::println("Accessed by forklift: {}", accessible);
}
