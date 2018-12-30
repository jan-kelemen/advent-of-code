#include <array>
#include <iostream>

constexpr int grid_dimension = 300;

[[nodiscard]] int power_level(int const x, int const y, int const grid_serial_number)
{
    auto const rack_id = x + 10;
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

    auto max = std::make_tuple(std::numeric_limits<int>::min(), std::numeric_limits<int>::min(), std::numeric_limits<int>::min());
    auto max_power = std::numeric_limits<int>::min();
    for (auto size = 0; size != grid_dimension; ++size)
    {
        std::cout << size << '\n';
        for (auto x = 0; x != grid_dimension - size; ++x)
        {
            for (auto y = 0; y != grid_dimension - size; ++y)
            {
                auto total_power = 0;
                for (auto square_x = x; square_x != x + size; ++square_x)
                {
                    for (auto square_y = y; square_y != y + size; ++square_y)
                    {
                        total_power += grid[square_x][square_y];
                    }
                }
                if (total_power < max_power) continue;

                max = std::make_tuple(x + 1, y + 1, size);
                max_power = total_power;
            }
        }
    }

    std::cout << std::get<0>(max) << ',' << std::get<1>(max) << ',' << std::get<2>(max) << ' ' << '\n';
}