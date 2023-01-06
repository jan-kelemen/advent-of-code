#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input {argv[1]};

    std::vector<std::vector<int>> grid {};

    std::string line;
    while (std::getline(input, line))
    {
        std::vector<int> row {};
        std::transform(std::cbegin(line),
            std::cend(line),
            std::back_inserter(row),
            [](auto&& c) { return c - '0'; });
        grid.push_back(std::move(row));
    }

    size_t visible_trees {};
    for (size_t i {0}; i != grid.size(); ++i)
    {
        for (size_t j {0}; j != grid[i].size(); ++j)
        {
            auto const visible_from_left = [&grid](auto const x, auto const y)
            {
                for (size_t i {0}; i != x; ++i)
                {
                    if (grid[i][y] >= grid[x][y])
                    {
                        return false;
                    }
                }
                return true;
            }(i, j);
            auto const visible_from_right = [&grid](auto const x, auto const y)
            {
                for (size_t i {x + 1}; i != grid[y].size(); ++i)
                {
                    if (grid[i][y] >= grid[x][y])
                    {
                        return false;
                    }
                }
                return true;
            }(i, j);
            auto const visible_from_top = [&grid](auto const x, auto const y)
            {
                for (size_t i {0}; i != y; ++i)
                {
                    if (grid[x][i] >= grid[x][y])
                    {
                        return false;
                    }
                }
                return true;
            }(i, j);
            auto const visible_from_bottom = [&grid](auto const x, auto const y)
            {
                for (size_t i {y + 1}; i != grid.size(); ++i)
                {
                    if (grid[x][i] >= grid[x][y])
                    {
                        return false;
                    }
                }
                return true;
            }(i, j);

            visible_trees += visible_from_left || visible_from_right ||
                visible_from_top || visible_from_bottom;
        }
    }

    std::cout << visible_trees << '\n';
}
