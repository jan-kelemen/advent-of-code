#include <fmt/format.h>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>

template<typename Op>
uint64_t calc(std::vector<std::vector<uint64_t>> const& numbers,
    size_t i,
    uint64_t init,
    Op op)
{
    uint64_t rv{init};
    for (auto const& n : numbers)
    {
        rv = op(rv, n[i]);
    }
    return rv;
}

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    std::regex const number_regex{R"((\d+))"};

    std::vector<std::vector<uint64_t>> numbers;

    std::string line;
    while (std::getline(input, line))
    {
        if (line.starts_with('+') || line.starts_with('-'))
            break;

        auto& current{numbers.emplace_back()};

        std::cregex_iterator const end;
        for (std::cregex_iterator it{line.c_str(),
                 line.c_str() + line.size(),
                 number_regex};
            it != end;
            ++it)
        {
            std::cmatch const& match{*it};
            current.push_back(std::stoull(match.str()));
        }
    }

    uint64_t total{};
    size_t i{};
    std::regex const symbol_regex{R"(\+|\*)"};
    std::cregex_iterator const end;
    for (std::cregex_iterator it{line.c_str(),
             line.c_str() + line.size(),
             symbol_regex};
        it != end;
        ++it, ++i)
    {
        std::cmatch const& match{*it};
        if (auto const sym{match.str()}; sym == "+")
        {
            total += calc(numbers, i, 0, std::plus{});
        }
        else
        {
            total += calc(numbers, i, 1, std::multiplies{});
        }
    }

    fmt::println("Total: {}", total);
}
