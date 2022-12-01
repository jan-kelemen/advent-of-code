#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <utility>

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input {argv[1]};

    std::multiset<std::uint64_t, std::greater<>> calorie_counts;

    std::uint64_t current_calories {};
    std::string line;
    while (std::getline(input, line))
    {
        if (line.empty())
        {
            calorie_counts.insert(std::exchange(current_calories, 0));
            continue;
        }
        current_calories += std::stoull(line);
    }

    auto const elfs_with_most_calories =
        std::accumulate(std::cbegin(calorie_counts),
            std::next(std::cbegin(calorie_counts), 3),
            0);

    std::cout << elfs_with_most_calories << '\n';
}
