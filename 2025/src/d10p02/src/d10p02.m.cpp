#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <execution>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <regex>
#include <vector>

struct [[nodiscard]] machine_configuration_t
{
    uint64_t lights{};
    std::vector<uint64_t> buttons;
    std::vector<uint64_t> joltage;
};

machine_configuration_t bracket_split(std::string_view str)
{
    static std::regex const number_regex{R"((\d+)+)"};

    machine_configuration_t rv;
    for (auto it{str.cbegin()}; it != str.cend(); ++it)
    {
        switch (*it)
        {
        case '[':
        {
            auto begin{++it};
            while (*it != ']')
            {
                if (*it == '#')
                {
                    rv.lights |= uint64_t{1} << std::distance(begin, it);
                }
                ++it;
            }
            break;
        }
        case '(':
        {
            auto& button{rv.buttons.emplace_back()};
            auto const end{std::find(++it, str.cend(), ')')};
            std::string omg{it, end};
            for (std::smatch sm; std::regex_search(omg, sm, number_regex);)
            {
                int bit{std::stoi(std::string{sm[0]})};
                button |= uint64_t{1} << bit;
                omg = sm.suffix();
            }
            break;
        }
        case '{':
        {
            auto const end{std::find(++it, str.cend(), '}')};
            std::string omg{it, end};
            for (std::smatch sm; std::regex_search(omg, sm, number_regex);)
            {
                rv.joltage.push_back(std::stoi(std::string{sm[0]}));
                omg = sm.suffix();
            }
            break;
        }
        }
    }

    return rv;
}

struct [[nodiscard]] button_limit_t
{
    uint64_t lower_limit{0};
    uint64_t upper_limit{std::numeric_limits<uint64_t>::max()};
};

uint16_t bad_idea(
    std::function<uint16_t(std::vector<uint16_t> const&)> const& evaluate_seed,
    std::vector<button_limit_t> const& limits,
    size_t const button,
    std::vector<uint16_t> seed)
{
    if (button >= limits.size())
    {
        return evaluate_seed(seed);
    }

    uint16_t rv{std::numeric_limits<uint16_t>::max()};
    for (uint64_t i{limits[button].lower_limit};
        i != limits[button].upper_limit + 1;
        ++i)
    {
        seed.push_back(i);
        rv = std::min(rv, bad_idea(evaluate_seed, limits, button + 1, seed));
        seed.pop_back();
    }
    return rv;
}

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    std::vector<machine_configuration_t> configurations;

    std::string line;
    while (std::getline(input, line))
    {
        configurations.push_back(bracket_split(line));
    }

    std::atomic<uint64_t> area{0};
    std::for_each(std::execution::par_unseq,
        configurations.cbegin(),
        configurations.cend(),
        [&area](machine_configuration_t const& configuration)
        {
            std::vector<button_limit_t> button_limits(
                configuration.buttons.size());

            for (size_t const bit :
                std::views::iota(size_t{0}, configuration.joltage.size()))
            {
                std::vector<size_t> responsible_buttons;

                for (size_t i{}; i != button_limits.size(); ++i)
                {
                    if (configuration.buttons[i] & uint64_t{1} << bit)
                    {
                        button_limits[i].upper_limit =
                            std::min(button_limits[i].upper_limit,
                                configuration.joltage[bit]);

                        responsible_buttons.push_back(i);
                    }
                }

                if (responsible_buttons.size() == 1)
                {
                    button_limits[responsible_buttons.front()].lower_limit =
                        configuration.joltage[bit];
                    button_limits[responsible_buttons.front()].upper_limit =
                        configuration.joltage[bit];
                }
            }

            fmt::println("{} {}",
                std::this_thread::get_id(),
                std::ranges::fold_left(button_limits,
                    uint64_t{1},
                    [](auto acc, auto const& limits)
                    {
                        return acc *=
                            (limits.upper_limit - limits.lower_limit + 1);
                    }));

            area += bad_idea(
                [&configuration, evaluation_joltages = configuration.joltage](
                    std::vector<uint16_t> const& presses) mutable -> uint16_t
                {
                    std::ranges::copy(configuration.joltage,
                        evaluation_joltages.begin());

                    bool is_invalid{false};
                    for (auto const& [press, button] :
                        std::views::zip(presses, configuration.buttons))
                    {
                        for (size_t const bit : std::views::iota(size_t{0},
                                 configuration.joltage.size()))
                        {
                            if (button & uint64_t{1} << bit)
                            {
                                if (evaluation_joltages[bit] < press)
                                {
                                    return std::numeric_limits<uint16_t>::max();
                                }
                                evaluation_joltages[bit] -= press;
                            }
                        }
                    }
                    if (std::ranges::fold_left(evaluation_joltages,
                            0,
                            std::plus{}) == 0)
                    {
                        return std::ranges::fold_left(presses,
                            uint16_t{0},
                            std::plus{});
                    }
                    return std::numeric_limits<uint16_t>::max();
                },
                button_limits,
                0,
                {});
        });

    fmt::println("Sum: {}", area.load());
}
