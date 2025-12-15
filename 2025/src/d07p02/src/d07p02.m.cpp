#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <future>
#include <iostream>
#include <iterator>
#include <map>
#include <ranges>
#include <vector>

enum class field : char
{
    manifold = 'S',
    empty = '.',
    splitter = '^',
};

[[nodiscard]] size_t descend(std::vector<std::vector<field>> const& diagram,
    std::pair<size_t, size_t> const& position,
    std::map<std::pair<size_t, size_t>, size_t>& cache)
{
    if (position.first + 1 == diagram.size())
    {
        return 1;
    }

    if (auto const it{
            cache.find(std::make_pair(position.first + 1, position.second))};
        it != cache.cend())
    {
        return it->second;
    }

    if (diagram[position.first + 1][position.second] == field::splitter)
    {
        auto const rv{
            descend(diagram,
                std::make_pair(position.first + 1, position.second - 1),
                cache) +
            descend(diagram,
                std::make_pair(position.first + 1, position.second + 1),
                cache)};

        cache[std::make_pair(position.first + 1, position.second)] = rv;

        return rv;
    }

    auto const rv{descend(diagram,
        std::make_pair(position.first + 1, position.second),
        cache)};

    cache[std::make_pair(position.first + 1, position.second)] = rv;

    return rv;
}

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
    std::map<std::pair<size_t, size_t>, size_t> cache;
    size_t const total{descend(diagram,
        std::make_pair(0,
            std::distance(diagram[0].begin(),
                std::ranges::find(diagram[0], field::manifold))),
        cache)};

    fmt::println("Total: {}", total);
}
