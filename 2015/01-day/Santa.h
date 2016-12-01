#ifndef SANTA_H

#define SANTA_H

#include <exception>
#include <string>

using floor_t = int;
using stairs_t = std::string;

class Santa
{
public:
    Santa() = default;

    floor_t walk(const stairs_t &stairs);

    size_t walk_to_floor(const stairs_t &stairs, const floor_t &floor);

    ~Santa() = default;
};

#endif //SANTA_H