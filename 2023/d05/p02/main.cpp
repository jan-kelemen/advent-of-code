#include <algorithm>
#include <cstdint>
#include <iostream>
#include <lexy/callback/container.hpp>
#include <limits>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>

#include <lexy/action/parse.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

namespace ast
{
    struct map_range
    {
        int64_t destination_range_start;
        int64_t source_range_start;
        int64_t length;
    };

    struct map_range_cmp
    {
        bool operator()(map_range const& lhs, map_range const& rhs) const
        {
            return lhs.source_range_start < rhs.source_range_start;
        }
    };

    struct almanac
    {
        std::vector<int64_t> seeds;
        std::set<map_range, map_range_cmp> seed_to_soil;
        std::set<map_range, map_range_cmp> soil_to_fertilizer;
        std::set<map_range, map_range_cmp> fertilizer_to_water;
        std::set<map_range, map_range_cmp> water_to_light;
        std::set<map_range, map_range_cmp> light_to_temperature;
        std::set<map_range, map_range_cmp> temperature_to_humidity;
        std::set<map_range, map_range_cmp> humidity_to_location;
    };
} // namespace ast

namespace grammar
{
    namespace dsl = lexy::dsl;

    struct map_range
    {
        static constexpr auto rule = dsl::integer<int64_t> +
            dsl::integer<int64_t> + dsl::integer<int64_t>;

        static constexpr auto value = lexy::construct<ast::map_range>;
    };

    struct map
    {
        static constexpr auto rule =
            dsl::list(dsl::peek(dsl::digits<>) >> dsl::p<map_range>);

        static constexpr auto value =
            lexy::as_collection<std::set<ast::map_range, ast::map_range_cmp>>;
    };

    struct number_list
    {
        static constexpr auto rule = dsl::list(dsl::integer<int64_t>);

        static constexpr auto value = lexy::as_list<std::vector<int64_t>>;
    };

    struct almanac
    {
        static constexpr auto whitespace = dsl::ascii::space;

        static constexpr auto rule = []()
        {
            return LEXY_LIT("seeds: ") + dsl::p<number_list> +
                LEXY_LIT("seed-to-soil map:") + dsl::p<map> +
                LEXY_LIT("soil-to-fertilizer map:") + dsl::p<map> +
                LEXY_LIT("fertilizer-to-water map:") + dsl::p<map> +
                LEXY_LIT("water-to-light map:") + dsl::p<map> +
                LEXY_LIT("light-to-temperature map:") + dsl::p<map> +
                LEXY_LIT("temperature-to-humidity map:") + dsl::p<map> +
                LEXY_LIT("humidity-to-location map:") + dsl::p<map>;
        }();

        static constexpr auto value = lexy::construct<ast::almanac>;
    };
} // namespace grammar

std::pair<int64_t, size_t> map_to_destination(
    std::set<ast::map_range, ast::map_range_cmp> const& map,
    int64_t value)
{
    auto const is_in_mapped_range{[&value](ast::map_range const& r)
        {
            return value >= r.source_range_start &&
                value < r.source_range_start + r.length;
        }};
    auto it{std::find_if(map.cbegin(), map.cend(), is_in_mapped_range)};

    if (it == map.cend())
    {
        auto bound = std::lower_bound(map.cbegin(),
            map.cend(),
            value,
            [](ast::map_range const& r, int64_t value)
            {
                return value >= r.source_range_start &&
                    value < r.source_range_start + r.length;
            });
        if (bound == map.cend())
        {
            return {value, std::numeric_limits<int64_t>::max() - value};
        }

        return {value, bound->source_range_start - value};
    }

    return {(value - it->source_range_start) + it->destination_range_start,
        it->length - (value - it->source_range_start)};
}

std::pair<int64_t, int64_t> map_from_almanac(ast::almanac const& almanac,
    int64_t value)
{
    auto tmp = map_to_destination(almanac.seed_to_soil, value);
    auto safe_skip = tmp.second;
    tmp = map_to_destination(almanac.soil_to_fertilizer, tmp.first);
    safe_skip = std::min(safe_skip, tmp.second);
    tmp = map_to_destination(almanac.fertilizer_to_water, tmp.first);
    safe_skip = std::min(safe_skip, tmp.second);
    tmp = map_to_destination(almanac.water_to_light, tmp.first);
    safe_skip = std::min(safe_skip, tmp.second);
    tmp = map_to_destination(almanac.light_to_temperature, tmp.first);
    safe_skip = std::min(safe_skip, tmp.second);
    tmp = map_to_destination(almanac.temperature_to_humidity, tmp.first);
    safe_skip = std::min(safe_skip, tmp.second);
    tmp = map_to_destination(almanac.humidity_to_location, tmp.first);
    safe_skip = std::min(safe_skip, tmp.second);

    return {tmp.first, safe_skip};
}

int main([[maybe_unused]] int argc, char** argv)
{
    auto file{lexy::read_file(argv[1])};

    auto almanac{
        lexy::parse<grammar::almanac>(file.buffer(), lexy_ext::report_error)};

    auto const chunks = almanac.value().seeds | std::views::chunk(2);

    std::cout << std::transform_reduce(
                     chunks.cbegin(),
                     chunks.cend(),
                     std::numeric_limits<int64_t>::max(),
                     [](auto lhs, auto rhs) { return std::min(lhs, rhs); },
                     [&almanac](std::ranges::viewable_range auto&& chunk_range)
                     {
                         int64_t rv{std::numeric_limits<int64_t>::max()};
                         for (int64_t i{chunk_range[0]};
                              i <= chunk_range[0] + chunk_range[1];)
                         {
                             auto current{map_from_almanac(almanac.value(), i)};
                             rv = std::min(current.first, rv);
                             i += current.second;
                         }
                         return rv;
                     })
              << '\n';
}
