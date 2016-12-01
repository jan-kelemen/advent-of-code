#ifndef CONTROLLABLE_LIGHT_GRID

#define CONTROLLABLE_LIGHT_GRID

#include <vector>

#include "LightGrid.h"
class ControllableLightGrid : public LightGrid
{
public:
    ControllableLightGrid();

    virtual void turn_on(const coordinate_t &coordinate) override;

    virtual void turn_off(const coordinate_t &coordinate) override;

    virtual void toggle(const coordinate_t &coordinate) override;

    virtual size_t turned_on() const override;

    ~ControllableLightGrid() override = default;
private:
    std::vector<std::vector<uint32_t>> grid;
};

#endif //CONTROLLABLE_LIGHT_GRID