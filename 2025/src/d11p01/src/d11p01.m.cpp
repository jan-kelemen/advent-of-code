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

int descend(
    std::map<std::string, std::vector<std::string>, std::less<>> const& devices,
    std::string const& name,
    std::string const& search_for)
{
    auto it{devices.find(name)};
    if (it == devices.cend())
    {
        return 0;
    }

    auto const& connections{it->second};
    if (std::ranges::contains(connections, search_for))
    {
        return 1;
    }

    return std::ranges::fold_left(connections,
        0,
        [&devices, &search_for](int acc, std::string const& next)
        { return acc + descend(devices, next, search_for); });
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

    int area = descend(devices, "you", "out");

    fmt::println("Sum: {}", area);
}
