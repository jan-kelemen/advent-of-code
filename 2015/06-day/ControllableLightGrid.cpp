#include <algorithm>

#include "ControllableLightGrid.h"

ControllableLightGrid::ControllableLightGrid() : LightGrid()
{
    for (auto i = 0; i < grid_dimension; ++i)
    {
        grid.push_back(std::vector<uint32_t>(grid_dimension));
    }
}

void ControllableLightGrid::turn_on(const coordinate_t &coordinate)
{
    LightGrid::turn_on(coordinate);
    grid[coordinate.first][coordinate.second]++;
}

void ControllableLightGrid::turn_off(const coordinate_t &coordinate)
{
    auto new_value = std::max<int32_t>(0, grid[coordinate.first][coordinate.second] - 1);

    if (new_value == 0)
    {
        LightGrid::turn_off(coordinate);
    }

    grid[coordinate.first][coordinate.second] = new_value;
}

void ControllableLightGrid::toggle(const coordinate_t &coordinate)
{
    LightGrid::turn_on(coordinate);
    grid[coordinate.first][coordinate.second] += 2;
}

size_t ControllableLightGrid::turned_on() const
{
    auto total = size_t(0);

    for (const auto &row : grid)
    {
        for (const auto &element : row)
        {
            total += element;
        }
    }

    return total;
}