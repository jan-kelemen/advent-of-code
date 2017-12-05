#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> read_instructions(std::istream& stream);

int calculate_steps(std::vector<int>& instructions);

int main(int argc, char** argv)
{
    auto instructions = read_instructions(std::cin);
    auto steps = calculate_steps(instructions);
    std::cout << steps << '\n';
}

std::vector<int> read_instructions(std::istream& stream)
{
    return std::vector<int>(std::istream_iterator<int>(stream), {});
}

int calculate_steps(std::vector<int>& instructions)
{
    auto count = 0;
    size_t position = 0;
    while(position < instructions.size())
    {
        auto old_position = position;
        position += instructions[position];
        
        if(instructions[old_position] >= 3) { --instructions[old_position]; }
        else { ++instructions[old_position]; }

        ++count;
    }
    return count;
}
