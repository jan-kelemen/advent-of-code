#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <set>
#include <vector>

enum class field : char
{
    manifold = 'S',
    empty = '.',
    splitter = '^',
};

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};
    std::vector<std::vector<field>> diagram;

    std::string line;
    while (std::getline(input, line))
    {
        std::ranges::transform(line,
            std::back_inserter(diagram.emplace_back()),
            [](char const c) { return static_cast<field>(c); });
    }

    std::set<std::pair<size_t, size_t>> retired_beams;
    std::set<std::pair<size_t, size_t>> active_beams;

    active_beams.emplace(0,
        std::distance(diagram[0].begin(),
            std::ranges::find(diagram[0], field::manifold)));

    for (size_t i{1}; i != diagram.size(); ++i)
    {
        std::set<std::pair<size_t, size_t>> promoted_beams;
        for (auto const& [y, x] : active_beams)
        {
            if (diagram[i][x] == field::splitter)
            {
                retired_beams.emplace(y, x);
                promoted_beams.emplace(i, x - 1);
                promoted_beams.emplace(i, x + 1);
            }
            else
            {
                assert(diagram[i][x] == field::empty);
                promoted_beams.emplace(i, x);
            }
        }
        active_beams = promoted_beams;
    }
    fmt::println("Total: {}", retired_beams.size());
}
