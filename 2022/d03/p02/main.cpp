#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <string>
#include <vector>

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input {argv[1]};

    std::vector<uint64_t> priorities;

    std::string rucksack1, rucksack2, rucksack3;
    while (std::getline(input, rucksack1) && std::getline(input, rucksack2) &&
        std::getline(input, rucksack3))
    {
        std::sort(std::begin(rucksack1), std::end(rucksack1));
        std::sort(std::begin(rucksack2), std::end(rucksack2));
        std::sort(std::begin(rucksack3), std::end(rucksack3));

        std::set<char> first_intersect, second_intersect, final_intersect;
        std::set_intersection(std::cbegin(rucksack1),
            std::cend(rucksack1),
            std::cbegin(rucksack2),
            std::cend(rucksack2),
            std::inserter(first_intersect, std::end(first_intersect)));
        std::set_intersection(std::cbegin(rucksack2),
            std::cend(rucksack2),
            std::cbegin(rucksack3),
            std::cend(rucksack3),
            std::inserter(second_intersect, std::end(second_intersect)));
        std::set_intersection(std::cbegin(first_intersect),
            std::cend(first_intersect),
            std::cbegin(second_intersect),
            std::cend(second_intersect),
            std::inserter(final_intersect, std::end(final_intersect)));

        if (auto badge {*std::cbegin(final_intersect)}; islower(badge))
        {
            priorities.push_back(static_cast<uint64_t>(badge - 'a' + 1));
        }
        else
        {
            priorities.push_back(static_cast<uint64_t>(badge - 'A' + 27));
        }
    }

    auto const sum_of_priorities = std::accumulate(std::cbegin(priorities),
        std::cend(priorities),
        uint64_t {0});

    std::cout << sum_of_priorities << '\n';
}
