#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    int position{50};

    uint32_t password{0};

    char direction;
    int count;
    while (input >> direction >> count)
    {
        if (direction == 'L')
        {
            position -= count;
        }
        else
        {
            assert(direction == 'R');
            position += count;
        }

        position = position % 100;
        if (position < 0)
        {
            position += 100;
        }

        password += static_cast<uint32_t>(position == 0);

        fmt::println("{} {} {}", direction, count, position);
    }

    fmt::println("password is: {}", password);
}
