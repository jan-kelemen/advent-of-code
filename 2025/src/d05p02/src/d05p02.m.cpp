#include <boost/icl/split_interval_map.hpp>

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

    boost::icl::split_interval_map<uint64_t, uint64_t> overlaps;

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

        overlaps += std::make_pair(
            boost::icl::interval<uint64_t>::closed(begin_id, end_id),
            uint64_t{1});
    }

    uint64_t super_fresh{};
    for (auto const& [range, count] : overlaps)
    {
        if (count > 1)
            super_fresh += range.upper() - range.lower() + 1;
    }

    fmt::println("Super fresh: {}", super_fresh);
}
