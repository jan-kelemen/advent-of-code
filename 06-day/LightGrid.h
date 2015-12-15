#ifndef LIGHT_GRID_H

#define LIGHT_GRID_H

#include <bitset>

const size_t grid_dimension = 1000;

using coordinate_t = std::pair<size_t, size_t>;

class LightGrid
{
public: 
    LightGrid() = default;

    virtual void turn_on(const coordinate_t &coordinate);

    virtual void turn_off(const coordinate_t &coodrinate);

    virtual void toggle(const coordinate_t &coordinate);

    virtual size_t turned_on() const;

    virtual size_t turned_off() const;

    virtual ~LightGrid() = default;

private:
    std::bitset<grid_dimension * grid_dimension> grid;
};

#endif // LIGHT_GRID_H