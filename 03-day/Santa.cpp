#include <exception>

#include "Santa.h"

void Santa::deliver_by_directions(const directions_t &directions, Block &block)
{
    auto current = coordinate_t();
    block.leave_present(current);

    for (const auto &command : directions)
    {
        if (command == '^')
        {
            ++current.second;
        }
        else if (command == 'v')
        {
            --current.second;
        }
        else if (command == '<')
        {
            --current.first;
        }
        else if (command == '>')
        {
            ++current.first;
        }
        else
        {
            throw std::invalid_argument("Directions contain a unknown direction.");
        }

        block.leave_present(current);
    }
}
