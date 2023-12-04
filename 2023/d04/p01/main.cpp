#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>

#include <lexy/action/parse.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/dsl/punctuator.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

namespace ast
{
    struct card
    {
        int number;
        std::set<int> winning_numbers;
        std::set<int> my_numbers;
    };
} // namespace ast

namespace grammar
{
    namespace dsl = lexy::dsl;

    struct number_list
    {
        static constexpr auto rule = dsl::list(dsl::integer<int>);

        static constexpr auto value = lexy::as_collection<std::set<int>>;
    };

    struct card
    {
        static constexpr auto rule = dsl::integer<int> + dsl::lit_c<':'> +
            dsl::p<number_list> + dsl::lit_c<'|'> + dsl::p<number_list>;

        static constexpr auto value = lexy::construct<ast::card>;
    };

    struct cards
    {
        static constexpr auto whitespace = dsl::ascii::space;

        static constexpr auto rule =
            dsl::list(LEXY_LIT("Card") >> dsl::p<card>);

        static constexpr auto value = lexy::as_list<std::vector<ast::card>>;
    };
} // namespace grammar

int main([[maybe_unused]] int argc, char** argv)
{
    auto file{lexy::read_file(argv[1])};

    auto games{
        lexy::parse<grammar::cards>(file.buffer(), lexy_ext::report_error)};

    std::cout << std::transform_reduce(games.value().cbegin(),
                     games.value().cend(),
                     std::size_t{},
                     std::plus<>{},
                     [](auto const& c)
                     {
                         std::vector<int> intersect;
                         std::set_intersection(c.winning_numbers.cbegin(),
                             c.winning_numbers.cend(),
                             c.my_numbers.cbegin(),
                             c.my_numbers.cend(),
                             std::back_inserter(intersect));
                         return intersect.empty()
                             ? 0
                             : std::pow(2, intersect.size() - 1);
                     })
              << '\n';
}
