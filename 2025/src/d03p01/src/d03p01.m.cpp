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
        auto const first{std::max_element(line.cbegin(), line.cend() - 1)};
        auto const second{std::max_element(first + 1, line.cend())};

        total_output += (*first - '0') * 10 + (*second - '0');
    }

    fmt::println("Total output is: {}", total_output);
}
