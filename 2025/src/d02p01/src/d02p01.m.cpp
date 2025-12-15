#include <fmt/format.h>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>

int magnitude(unsigned long long n)
{
    return std::floor(std::log10(static_cast<double>(n)) + 1);
}

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    std::regex const range_regex{R"((\d+)-(\d+))"};

    unsigned long long invalid_sum{};

    std::string line;
    while (std::getline(input, line))
    {
        std::sregex_iterator const end;
        for (std::sregex_iterator it{line.begin(), line.end(), range_regex};
            it != end;
            ++it)
        {
            std::smatch const& match = *it;

            unsigned long long const begin_id{std::stoull(match[1].str())};
            unsigned long long const end_id{std::stoull(match[2].str())};

            for (auto const id : std::views::iota(begin_id, end_id + 1))
            {
                int const id_magnitude{magnitude(id)};
                if (id_magnitude % 2)
                {
                    continue;
                }

                auto const factor{static_cast<unsigned long long>(std::round(
                    std::pow(10.0, static_cast<double>(id_magnitude) / 2.0)))};

                if (id / factor == id % factor)
                {
                    invalid_sum += id;
                }
            }
        }
    }

    fmt::println("Invalid ID sum is: {}", invalid_sum);
}
