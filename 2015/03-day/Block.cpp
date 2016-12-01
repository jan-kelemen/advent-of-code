#include "Block.h"

void Block::leave_present(const coordinate_t &coordinate)
{
    ++left_presents[coordinate];
}