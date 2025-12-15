#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
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

    uint64_t total_removed{};

    std::vector<std::pair<int, int>> accessible;
    do
    {
        total_removed += accessible.size();
        for (auto const& [i, j] : accessible)
        {
            rolls[i][j] = false;
        }
        accessible.clear();

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

                if (surrounding < 4)
                {
                    accessible.emplace_back(i, j);
                }
            }
        }
    } while (!accessible.empty());

    std::print("Accessed by forklift: {}", total_removed);
}
