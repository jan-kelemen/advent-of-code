#include <cstdint>
#include <fstream>
#include <iostream>
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

    auto const elf_with_most_calories = *calorie_counts.begin();

    std::cout << elf_with_most_calories << '\n';
}
