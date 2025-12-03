#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    unsigned long long total_output{};

    std::string line;
    while (std::getline(input, line))
    {
        unsigned long long intermediate{};
        auto begin{line.cbegin()};
        auto end{line.cend() - 11};
        while (true)
        {
            fmt::println("{}", std::string_view{begin, end});
            auto const digit{std::max_element(begin, end)};
            intermediate *= 10;
            intermediate += (*digit - '0');

            if (end == line.cend())
            {
                break;
            }

            begin = digit + 1;
            ++end;
        }

        fmt::println("{}", intermediate);

        total_output += intermediate;
    }

    fmt::println("Total output is: {}", total_output);
}
