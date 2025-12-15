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

                int const max_count{id_magnitude / 2};
                for (int i{1}; i != max_count + 1; ++i)
                {
                    if (id_magnitude % i)
                    {
                        continue;
                    }

                    auto const factor{static_cast<unsigned long long>(
                        std::round(std::pow(10.0, i)))};

                    unsigned long long const sequence{id % factor};

                    bool sequence_match{true};
                    unsigned long long remaining{id / factor};
                    while (remaining > 0)
                    {
                        if (remaining % factor != sequence)
                        {
                            sequence_match = false;
                            break;
                        }
                        remaining /= factor;
                    }

                    if (sequence_match)
                    {
                        invalid_sum += id;
                        break;
                    }
                }
            }
        }
    }

    fmt::println("Invalid ID sum is: {}", invalid_sum);
}
