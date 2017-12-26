#include <algorithm>
#include <iostream>

enum class direction { left, right, up, down };

int read_starting_square_value(std::istream& stream);

int distance_to_center(int const starting_value);

int main(int argc, char** argv)
{
    auto starting_square = read_starting_square_value(std::cin);
    auto distance = distance_to_center(starting_square);
    std::cout << distance << '\n';
}

int read_starting_square_value(std::istream& stream)
{
    auto rv = 0;
    stream >> rv;
    return rv;
}

void adjust_coordinates(int& x, int& y, direction const direction)
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
}

void adjust_direction(direction& direction)
{
    switch(direction)
    {
    case direction::left:
        direction = direction::up;
        break;
    case direction::right:
        direction = direction::down;
        break;
    case direction::up:
        direction = direction::right;
        break;
    case direction::down:
        throw direction;
    default:
        break;
    }
}

int distance_to_center(int const starting_value)
{
    auto radius = 0;
    auto square = 0;
    while((square = std::pow(2 * radius + 1, 2)) < starting_value) { ++radius; }

    auto value = square;
    auto x = radius, y = radius;
    auto direction = direction::left;
    while(starting_value != value)
    {
        adjust_coordinates(x, y, direction);
        --value;
        if((square - value) % (2 * radius) == 0) { adjust_direction(direction); }
    }

    return std::abs(x) + std::abs(y);
}