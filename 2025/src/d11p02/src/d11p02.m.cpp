#include <fmt/format.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#include <ranges>
#include <regex>
#include <vector>

[[nodiscard]] uint64_t descend(
    std::map<std::string, std::vector<std::string>, std::less<>> const& devices,
    std::map<std::tuple<std::string_view, bool, bool>, uint64_t>& cache,
    std::string const& name,
    bool seen_dac,
    bool seen_fft,
    std::string const& search_for)
{
    if (name == "out")
    {
        return seen_dac && seen_fft ? 1 : 0;
    }
    else if (name == "dac")
    {
        seen_dac = true;
    }
    else if (name == "fft")
    {
        seen_fft = true;
    }

    if (auto const cache_it{cache.find(
            std::make_tuple(std::string_view{name}, seen_dac, seen_fft))};
        cache_it != cache.cend())
    {
        return cache_it->second;
    }

    auto const it{devices.find(name)};
    assert(it != devices.cend());

    auto const rv{std::ranges::fold_left(it->second,
        0,
        [&](uint64_t acc, std::string const& next)
        {
            return acc +
                descend(devices, cache, next, seen_dac, seen_fft, search_for);
        })};
    cache.emplace(std::make_tuple(std::string_view{name}, seen_dac, seen_fft),
        rv);

    return rv;
}

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    std::map<std::string, std::vector<std::string>, std::less<>> devices;

    std::string line;
    while (std::getline(input, line))
    {
        std::istringstream ss{line};
        std::string key;
        ss >> key;
        key.pop_back();

        auto& node{devices[key]};

        std::string value;
        while (ss >> value)
        {
            node.push_back(std::move(value));
        }
    }

    std::map<std::tuple<std::string_view, bool, bool>, uint64_t> cache;

    uint64_t area = descend(devices, cache, "svr", false, false, "out");

    fmt::println("Sum: {}", area);
}
