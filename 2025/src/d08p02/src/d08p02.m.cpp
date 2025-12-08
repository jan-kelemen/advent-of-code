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

    std::vector<std::pair<std::tuple<int, int, int>, std::vector<double>>>
        positions;

    std::string line;
    while (std::getline(input, line))
    {
        std::smatch match;
        bool const matched{std::regex_match(line.cbegin(),
            line.cend(),
            match,
            dimensions_regex)};
        assert(matched);

        positions.emplace_back(std::make_tuple(std::stoi(match[1]),
                                   std::stoi(match[2]),
                                   std::stoi(match[3])),
            std::vector<double>{});
    }

    for (size_t i{}; i != positions.size(); ++i)
    {
        positions[i].second.resize(positions.size());
        for (size_t j{}; j != positions.size(); ++j)
        {
            if (i == j)
            {
                continue;
            }
            auto const& x{positions[i].first};
            auto const& y{positions[j].first};

            double const distance{std::hypot(std::get<0>(x) - std::get<0>(y),
                std::get<1>(x) - std::get<1>(y),
                std::get<2>(x) - std::get<2>(y))};

            positions[i].second[j] = distance;
        }
    }

    std::pair<size_t, size_t> accepted;
    std::vector<std::set<size_t>> circuits;
    do
    {
        double min_distance{std::numeric_limits<double>::max()};

        for (size_t i{}; i != positions.size(); ++i)
        {
            for (size_t j{}; j != positions.size(); ++j)
            {
                if (auto const current_distance{positions[i].second[j]};
                    current_distance != 0.0 && current_distance < min_distance)
                {
                    min_distance = current_distance;
                    accepted = std::make_pair(i, j);
                }
            }
        }

        positions[accepted.first].second[accepted.second] = 0;
        positions[accepted.second].second[accepted.first] = 0;

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
    } while (
        !(circuits.size() == 1 && circuits.front().size() == positions.size()));

    fmt::println("Total: {}",
        std::get<0>(positions[accepted.first].first) *
            std::get<0>(positions[accepted.second].first));
}
