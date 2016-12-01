#include <iostream>
#include <fstream>

#include "Santa.h"

stairs_t readStairs(std::istream &input)
{
    auto stairs = stairs_t();

    if (input.good())
    {
        input >> stairs;
    }

    return stairs;
}
int main()
{
    auto input = std::ifstream("input.txt");
    auto stairs = readStairs(input);

    auto santa = Santa();
    auto floor = santa.walk(stairs);
    auto step = santa.walk_to_floor(stairs, floor_t(-1));

    std::cout << "Santa stopped on floor: " << floor << "\n";
    std::cout << "Santa walked to the basement on step: " << step << "\n";

    return EXIT_SUCCESS;
}