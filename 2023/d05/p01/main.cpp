#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
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

    struct almanac
    {
        std::vector<int64_t> seeds;
        std::vector<map_range> seed_to_soil;
        std::vector<map_range> soil_to_fertilizer;
        std::vector<map_range> fertilizer_to_water;
        std::vector<map_range> water_to_light;
        std::vector<map_range> light_to_temperature;
        std::vector<map_range> temperature_to_humidity;
        std::vector<map_range> humidity_to_location;
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
            lexy::as_list<std::vector<ast::map_range>>;
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

int64_t map_to_destination(std::vector<ast::map_range> const& map,
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
        return value;
    }

    return (value - it->source_range_start) + it->destination_range_start;
}

int64_t map_from_almanac(ast::almanac const& almanac, int64_t value)
{
    value = map_to_destination(almanac.seed_to_soil, value);
    value = map_to_destination(almanac.soil_to_fertilizer, value);
    value = map_to_destination(almanac.fertilizer_to_water, value);
    value = map_to_destination(almanac.water_to_light, value);
    value = map_to_destination(almanac.light_to_temperature, value);
    value = map_to_destination(almanac.temperature_to_humidity, value);
    return map_to_destination(almanac.humidity_to_location, value);
}

int main([[maybe_unused]] int argc, char** argv)
{
    auto file{lexy::read_file(argv[1])};

    auto almanac{
        lexy::parse<grammar::almanac>(file.buffer(), lexy_ext::report_error)};

    std::cout << std::transform_reduce(
                     almanac.value().seeds.cbegin(),
                     almanac.value().seeds.cend(),
                     std::numeric_limits<int64_t>::max(),
                     [](auto lhs, auto rhs) { return std::min(lhs, rhs); },
                     [&almanac](int64_t const& s)
                     { return map_from_almanac(almanac.value(), s); })
              << '\n';
}
