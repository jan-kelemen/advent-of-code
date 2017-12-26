#include <iostream>
#include <map>
#include <array>

enum class direction { left, right, up, down };

int read_starting_square_value(std::istream& stream);

int next_value(int const starting_value);

int main(int argc, char** argv)
{
    auto starting_square = read_starting_square_value(std::cin);
    auto next = next_value(starting_square);
    std::cout << next << '\n';
}

int read_starting_square_value(std::istream& stream)
{
    auto rv = 0;
    stream >> rv;
    return rv;
}

std::array<std::pair<int, int>, 8> adjecent_squares(int const x, int const y)
{
    return {
        std::make_pair(x + 1, y),
        std::make_pair(x + 1, y + 1),
        std::make_pair(x, y + 1),
        std::make_pair(x - 1, y + 1),
        std::make_pair(x - 1, y),
        std::make_pair(x - 1, y - 1),
        std::make_pair(x, y - 1),
        std::make_pair(x + 1, y - 1)
    };
}

void adjust(int& x, int& y, direction& direction)
{
    switch(direction)
    {
    case direction::left:
        --x;
        break;
    case direction::right:
        ++x;
        break;
    case direction::up:
        ++y;
        break;
    case direction::down:
        --y;
        break;
    default:
        break;
    }

    if(x == y) { direction = (x > 0 ? direction::left : direction::right); }
    else if(x < 0 && -x == y) { direction = direction::down; }
    else if(x > 0 && x - 1 == -y && direction == direction::right) { direction = direction::up; }
}

int next_value(int const starting_value)
{
    auto map = std::map<std::pair<int, int>, int>{
        { std::make_pair(0, 0), 1 }
    };
    auto x = 1, y = 0;
    auto direction = direction::up;

    auto current = 0;
    while(current < starting_value)
    {
        current = 0;
        for(auto const& square : adjecent_squares(x, y))
        {
            if(map.count(square))
            {
                current += map[square];
            }
        }
        map[{x, y}] = current;
        adjust(x, y, direction);
    }
    return current;
}
