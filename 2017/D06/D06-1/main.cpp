#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

std::vector<int> read_memory_bank(std::istream& stream);

int cycles_to_infinite_loop(std::vector<int> memory_bank);

int main(int argc, char** argv)
{
    auto memory_bank = read_memory_bank(std::cin);
    auto cycles = cycles_to_infinite_loop(memory_bank);
    std::cout << cycles << '\n';
}

std::vector<int> read_memory_bank(std::istream& stream)
{
    return std::vector<int>(std::istream_iterator<int>(stream), {});
}

int cycles_to_infinite_loop(std::vector<int> memory_bank)
{
    auto count = 0;
    auto seen = std::set<std::vector<int>>();
    while(!seen.count(memory_bank))
    {
        seen.insert(memory_bank);

        auto max_iterator = std::max_element(memory_bank.begin(), memory_bank.end());
        auto index = (std::distance(memory_bank.begin(), max_iterator) + 1) % memory_bank.size();
        auto blocks_to_realocate = *max_iterator;
        *max_iterator = 0;
        while(blocks_to_realocate != 0)
        {
            memory_bank[index] = std::max(0, memory_bank[index] + 1);
            --blocks_to_realocate;
            index = (index + 1) % memory_bank.size();
        }
        ++count;
    }
    return count;
}
