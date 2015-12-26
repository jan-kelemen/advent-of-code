#include <algorithm>

#include "LightGrid.h"

void LightGrid::turn_on(const coordinate_t &coordinate)
{
    auto coord = coordinate.first * grid_dimension + coordinate.second;

    grid.set(coord);
}

void LightGrid::turn_off(const coordinate_t &coordinate)
{
    auto coord = coordinate.first * grid_dimension + coordinate.second;

    grid.reset(coord);
}

void LightGrid::toggle(const coordinate_t &coordinate)
{
    auto coord = coordinate.first * grid_dimension + coordinate.second;

    grid.flip(coord);
}

size_t LightGrid::turned_on() const
{
    return grid.count();
}

size_t LightGrid::turned_off() const
{
    return grid_dimension * grid_dimension - turned_on();
}
