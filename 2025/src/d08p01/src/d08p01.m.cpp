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
#include <regex>
#include <set>
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

    static std::regex const dimensions_regex{R"((\d+),(\d+),(\d+))"};

    std::vector<std::pair<std::tuple<int, int, int>, std::monostate>> positions;

    std::string line;
    while (std::getline(input, line))
    {
        std::smatch match;
        [[maybe_unused]] bool const matched{std::regex_match(line.cbegin(),
            line.cend(),
            match,
            dimensions_regex)};
        assert(matched);

        positions.emplace_back(std::make_tuple(std::stoi(match[1]),
                                   std::stoi(match[2]),
                                   std::stoi(match[3])),
            std::monostate{});
    }

    std::vector<std::pair<double, std::pair<size_t, size_t>>> pairs_by_distance;

    for (size_t i{}; i != positions.size() - 1; ++i)
    {
        for (size_t j{i + 1}; j != positions.size(); ++j)
        {
            auto const& x{positions[i].first};
            auto const& y{positions[j].first};

            double const distance{std::hypot(std::get<0>(x) - std::get<0>(y),
                std::get<1>(x) - std::get<1>(y),
                std::get<2>(x) - std::get<2>(y))};

            pairs_by_distance.emplace_back(distance, std::make_pair(i, j));
        }
    }
    std::ranges::make_heap(pairs_by_distance,
        std::greater{},
        [](auto const& p) { return p.first; });

    std::vector<std::set<size_t>> circuits;

    for ([[maybe_unused]] int const connection : std::views::iota(0, 1000))
    {
        std::ranges::pop_heap(pairs_by_distance,
            std::greater{},
            [](auto const& p) { return p.first; });
        std::pair<size_t, size_t> accepted{pairs_by_distance.back().second};
        pairs_by_distance.pop_back();

        std::optional<size_t> circuit_of_first_point;
        std::optional<size_t> circuit_of_second_point;

        for (size_t const c : std::views::iota(size_t{0}, circuits.size()))
        {
            if (!circuit_of_first_point && circuits[c].contains(accepted.first))
            {
                circuit_of_first_point = c;
            }

            if (!circuit_of_second_point &&
                circuits[c].contains(accepted.second))
            {
                circuit_of_second_point = c;
            }

            if (circuit_of_first_point && circuit_of_second_point)
            {
                break;
            }
        }

        if (!circuit_of_first_point.has_value() &&
            !circuit_of_second_point.has_value())
        {
            std::set<size_t> new_circuit{accepted.first, accepted.second};
            circuits.emplace_back(std::move(new_circuit));
            continue;
        }

        if (circuit_of_first_point == circuit_of_second_point)
        {
            continue;
        }

        if (!circuit_of_first_point.has_value())
        {
            circuits[*circuit_of_second_point].insert(accepted.first);
            continue;
        }

        if (!circuit_of_second_point.has_value())
        {
            circuits[*circuit_of_first_point].insert(accepted.second);
            continue;
        }

        circuits[*circuit_of_first_point].merge(
            circuits[*circuit_of_second_point]);
        circuits.erase(circuits.begin() + *circuit_of_second_point);
    }

    std::ranges::nth_element(circuits,
        circuits.begin() + 4,
        std::greater<>{},
        [](auto const& c) { return c.size(); });
    size_t size_multiplication{
        circuits[0].size() * circuits[1].size() * circuits[2].size()};

    fmt::println("Total: {}", size_multiplication);
}
