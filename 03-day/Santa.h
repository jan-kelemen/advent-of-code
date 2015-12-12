#ifndef SANTA_H

#define SANTA_H

#include <string>

#include "Block.h"

using directions_t = std::string;

class Santa
{
public:
    Santa() = default;

    void deliver_by_directions(const directions_t &directions, Block &block);

    ~Santa() = default;
};

#endif // SANTA_H