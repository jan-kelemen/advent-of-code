#include <fstream>
#include <iostream>

#include "Santa.h"
#include "Block.h"

directions_t get_directions(std::istream &input)
{
    auto directions = directions_t();

    if (input.good())
    {
        input >> directions;
    }

    return directions;
}

std::pair<directions_t, directions_t> split_directions(const directions_t &directions)
{
    auto split_directions = std::pair<directions_t, directions_t>();

    for (size_t i = 0; i < directions.length(); ++i)
    {
        (i % 2 == 0 ? split_directions.first : split_directions.second) += directions[i];
    }

    return split_directions;
}

int main()
{
    auto directions = get_directions(std::ifstream("input.txt"));
    auto santa = Santa();
    auto block = Block();

    santa.deliver_by_directions(directions, block);

    std::cout << "Santa will deliver presents to " << block.visited_houses() << " houses.\n";

    auto new_directions = split_directions(directions);
    auto robo_santa = Santa();
    auto new_block = Block();

    santa.deliver_by_directions(new_directions.first, new_block);
    robo_santa.deliver_by_directions(new_directions.second, new_block);

    std::cout << "Santa and Robo-Santa will deliver presents to " << new_block.visited_houses() << " houses.\n";
    return EXIT_SUCCESS;
}