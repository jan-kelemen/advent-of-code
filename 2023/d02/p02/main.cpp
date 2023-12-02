#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <vector>

#include <lexy/action/parse.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/dsl/punctuator.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

struct cube_set
{
    std::uint64_t red{};
    std::uint64_t green{};
    std::uint64_t blue{};
};

struct game_set
{
    std::uint64_t id;
    std::vector<cube_set> sets;
};

namespace grammar
{
    namespace dsl = lexy::dsl;

    struct set
    {
        enum class color
        {
            red,
            blue,
            green
        };

        struct count_color
        {
            std::uint64_t count;
            enum color color;
        };

        struct individual
        {
            static constexpr auto colors =
                lexy::symbol_table<color>.map<LEXY_SYMBOL("red")>(color::red).map<LEXY_SYMBOL("blue")>(color::blue).map<LEXY_SYMBOL("green")>(color::green);

            static constexpr auto rule = []
            {
                auto color = dsl::symbol<colors>(dsl::identifier(dsl::ascii::alpha));
                return dsl::integer<std::uint64_t>(dsl::digits<>) + color;
            }();

            static constexpr auto value = lexy::construct<count_color>;
        };

        static constexpr auto rule = dsl::list(dsl::p<individual>, dsl::sep(dsl::comma));

        static constexpr auto value = lexy::as_list<std::vector<count_color>> >>
            lexy::callback<cube_set>(
                [](std::vector<count_color> const& dice)
                {
                    cube_set rv;
                    for (auto const& d : dice)
                    {
                        switch (d.color)
                        {
                        case color::red:
                            rv.red = d.count;
                            break;
                        case color::green:
                            rv.green = d.count;
                            break;
                        case color::blue:
                            rv.blue = d.count;
                            break;
                        }
                    }
                    return rv;
                });
    };

    struct game
    {
        struct individual
        {
            static constexpr auto rule = dsl::list(dsl::p<set>, dsl::sep(dsl::semicolon));

            static constexpr auto value = lexy::as_list<std::vector<cube_set>>;
        };

        static constexpr auto whitespace = dsl::ascii::space;

        static constexpr auto rule = dsl::integer<std::uint64_t> + dsl::lit_c<':'> + dsl::p<individual>;

        static constexpr auto value = lexy::construct<game_set>;
    };

    struct games
    {
        static constexpr auto rule = dsl::list(LEXY_LIT("Game") >> dsl::p<game>);

        static constexpr auto value = lexy::as_list<std::vector<game_set>>;
    };
} // namespace grammar

int main([[maybe_unused]] int argc, char** argv)
{
    auto file{lexy::read_file(argv[1])};

    auto games{lexy::parse<grammar::games>(file.buffer(), lexy_ext::report_error)};

    auto const transform = [](game_set const& game)
    {
        auto const red_max{std::max_element(game.sets.cbegin(),
            game.sets.cend(),
            [](cube_set const& lhs, cube_set const& rhs) {
                return lhs.red < rhs.red;
            })->red};
        auto const green_max{std::max_element(game.sets.cbegin(),
            game.sets.cend(),
            [](cube_set const& lhs, cube_set const& rhs) {
                return lhs.green < rhs.green;
            })->green};
        auto const blue_max{std::max_element(game.sets.cbegin(),
            game.sets.cend(),
            [](cube_set const& lhs, cube_set const& rhs) {
                return lhs.blue < rhs.blue;
            })->blue};

        return red_max * green_max * blue_max;
    };

    std::uint64_t power_sum{};
    for (auto const& g : games.value() | std::views::transform(transform))
    {
        power_sum += g;
    }
    std::cout << power_sum << '\n';
}
