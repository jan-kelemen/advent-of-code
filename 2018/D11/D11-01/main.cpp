#include <array>
#include <iostream>

constexpr auto grid_dimension = 300;
constexpr auto square_dimension = 3;

[[nodiscard]] int power_level(int const& x, int const& y, int const& grid_serial_number)
{
    auto rack_id = x + 10;
    return ((((rack_id * y + grid_serial_number) * rack_id) / 100) % 10) - 5;
}

int main()
{
    auto grid_serial_number = 0;
    std::cin >> grid_serial_number;

    auto grid = std::array<std::array<int, grid_dimension>, grid_dimension>{};
    for (auto x = 0; x != grid_dimension; ++x)
    {
        for (auto y = 0; y != grid_dimension; ++y)
        {
            grid[x][y] = power_level(x + 1, y + 1, grid_serial_number);
        }
    }

    auto max = std::make_pair(std::numeric_limits<int>::min(), std::numeric_limits<int>::min());
    auto max_power = std::numeric_limits<int>::min();

    for (auto x = 0; x != grid_dimension - square_dimension; ++x)
    {
        for (auto y = 0; y != grid_dimension - square_dimension; ++y)
        {
            auto total_power = 0;
            for (auto square_x = x; square_x != x + square_dimension; ++square_x)
            {
                for (auto square_y = y; square_y != y + square_dimension; ++square_y)
                {
                    total_power += grid[square_x][square_y];
                }
            }
            if (total_power < max_power) continue;
            max = std::make_pair(x + 1, y + 1);
            max_power = total_power;
        }
    }

    std::cout << max.first << ',' << max.second << '\n';
}