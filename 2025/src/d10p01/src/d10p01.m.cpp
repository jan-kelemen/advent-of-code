#include <fmt/format.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
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

    uint64_t area{0};
    for (machine_configuration_t& configuration : configurations)
    {
        uint64_t presses{std::numeric_limits<uint64_t>::max()};
        for (uint64_t const window :
            std::views::iota(uint64_t{1}, configuration.buttons.size()))
        {
            if (window > presses)
            {
                continue;
            }
            std::vector<uint8_t> enabled_buttons(window, uint8_t{1});
            enabled_buttons.resize(configuration.buttons.size());
            do
            {
                uint64_t these_presses{};
                for (auto const& [enabled, button] :
                    std::views::zip(enabled_buttons, configuration.buttons))
                {
                    these_presses ^= enabled * button;
                }

                if (these_presses == configuration.lights)
                {
                    presses = window;
                }
            } while (std::ranges::prev_permutation(enabled_buttons).found);
        }
        area += presses;
    }
    fmt::println("Sum: {}", area);
}
