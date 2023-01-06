#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input {argv[1]};

    std::vector<uint64_t> priorities;

    std::string rucksack;
    while (std::getline(input, rucksack))
    {
        auto half {std::next(std::begin(rucksack), rucksack.size() / 2)};
        std::sort(std::begin(rucksack), half);
        std::sort(half, std::end(rucksack));

        std::vector<char> intersect;
        std::set_intersection(std::begin(rucksack),
            half,
            half,
            std::end(rucksack),
            std::back_inserter(intersect));
        if (islower(intersect[0]))
        {
            priorities.push_back(static_cast<uint64_t>(intersect[0] - 'a' + 1));
        }
        else
        {
            priorities.push_back(
                static_cast<uint64_t>(intersect[0] - 'A' + 27));
        }
    }

    auto const sum_of_priorities = std::accumulate(std::cbegin(priorities),
        std::cend(priorities),
        uint64_t {0});

    std::cout << sum_of_priorities << '\n';
}
