#include <iostream>
#include <iterator>
#include <unordered_set>

int main()
{
    std::vector<int> frequency_changes{ std::istream_iterator<int>(std::cin), std::istream_iterator<int>() };
    std::unordered_set<int> previous_frequencies;
    int current_frequency = 0;

    std::size_t current = 0;
    while (previous_frequencies.insert(current_frequency).second)
    {
        current_frequency += frequency_changes[current];
        current = (current + 1) % frequency_changes.size();
    }
    std::cout << current_frequency << '\n';
}