#include "Santa.h"

floor_t Santa::walk(const stairs_t &stairs)
{
    auto current_floor = floor_t(0);

    for (const auto &stair : stairs)
    {
        if (stair == '(')
        {
            ++current_floor;
        }
        else if (stair == ')')
        {
            --current_floor;
        }
        else
        {
            throw std::invalid_argument("Stairs contain unknown symbol: " + stair);
        }
    }

    return current_floor;
}

size_t Santa::walk_to_floor(const stairs_t &stairs, const floor_t &floor)
{
    auto current_floor = floor_t(0);
    
    for (size_t i = 0; i < stairs.length(); ++i)
    {
        auto stair = stairs[i];

        if (stair == '(')
        {
            ++current_floor;
        }
        else if (stair == ')')
        {
            --current_floor;
        }
        else
        {
            throw std::invalid_argument("Stairs contain unknown symbol: " + stair);
        }

        if (current_floor == floor)
        {
            return i + 1;
        }
    }

    return -1;
}
