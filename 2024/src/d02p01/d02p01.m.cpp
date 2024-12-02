#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    std::vector<int64_t> numbers;

    uint64_t safe{};

    std::string line;
    while (std::getline(input, line))
    {
        numbers.resize(0);

        std::istringstream stream{std::move(line)};
        int64_t value;
        while (stream >> value)
        {
            numbers.push_back(value);
        }

        std::adjacent_difference(numbers.begin(),
            numbers.end(),
            numbers.begin());

        try
        {
            int64_t sign{numbers[1]};
            for (size_t i{1}; i != numbers.size(); ++i)
            {
                auto const v{numbers[i]};
                if (auto const abs{std::abs(v)}; abs < 1 || abs > 3)
                {
                    throw 1;
                }

                if (!(sign < 0 && numbers[i] < 0 || sign > 0 && numbers[i] > 0))
                {
                    throw 2;
                }
            }

            ++safe;
        }
        catch (...)
        {
        }
    }

    std::cout << safe << '\n';
}
