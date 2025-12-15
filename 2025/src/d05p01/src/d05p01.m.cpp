#include <boost/icl/interval_set.hpp>

#include <fmt/format.h>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    std::regex const range_regex{R"((\d+)-(\d+))"};

    boost::icl::interval_set<uint64_t> set;

    std::string line;
    while (std::getline(input, line))
    {
        if (line.empty())
            break;

        std::smatch match;
        [[maybe_unused]] bool const matched{
            std::regex_match(line, match, range_regex)};
        assert(matched);

        uint64_t const begin_id{std::stoull(match[1].str())};
        uint64_t const end_id{std::stoull(match[2].str())};
        fmt::println("{} {}", begin_id, end_id);

        set.insert(boost::icl::interval<uint64_t>::closed(begin_id, end_id));
    }

    uint64_t fresh{};
    while (std::getline(input, line))
    {
        uint64_t const id{std::stoull(line)};
        if (set.find(id) != set.end())
        {
            ++fresh;
        }
    }

    fmt::println("Part 1: {}", fresh);
    fmt::println("Part 2: {}", set.size());
}
