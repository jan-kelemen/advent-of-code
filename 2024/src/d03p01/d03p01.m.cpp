#include <algorithm>
#include <cassert>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    std::regex mul{R"(mul\((\d+),(\d+)\))"};

    uint64_t acc{};

    std::string line;
    while (std::getline(input, line))
    {
        std::sregex_iterator begin{line.cbegin(), line.cend(), mul};
        std::sregex_iterator end;

        for (auto i{begin}; i != end; ++i)
        {
            std::smatch match = *i;
            acc += std::stoull(match[1].str()) * std::stoull(match[2].str());
        }
    }

    std::cout << acc << '\n';
}
