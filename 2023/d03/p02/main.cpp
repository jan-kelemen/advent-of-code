#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <regex>
#include <string>
#include <utility>
#include <vector>

struct point
{
    int x;
    int y;

    friend auto operator<=>(point const&, point const&) = default;
};

std::vector<point> generate_points(point const& top_left,
    point const& bottom_right)
{
    std::vector<point> rv;
    for (int i{top_left.x}; i != bottom_right.x; ++i)
    {
        for (int j{top_left.y}; j != bottom_right.y; ++j)
        {
            rv.emplace_back(i, j);
        }
    }
    return rv;
}

std::optional<std::pair<point, point>> has_gear_ratio_adjecent(
    std::vector<std::string> const& schematic,
    int const row,
    int const position,
    int const length)
{
    int const upper_row{std::max(row - 1, 0)};
    int const lower_row{std::min(row + 2, static_cast<int>(schematic.size()))};
    int const left_column{std::max(position - 1, 0)};
    int const right_column{std::min(position + 1 + length,
        static_cast<int>(schematic[row].size()))};

    for (int i{upper_row}; i != lower_row; ++i)
    {
        for (int j{left_column}; j != right_column; ++j)
        {
            auto const chr{schematic[i][j]};
            if (chr == '*')
            {
                return std::make_pair(point{upper_row, left_column},
                    point{lower_row, right_column});
            }
        }
    }
    return std::nullopt;
}

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input{argv[1]};

    std::vector<std::string> schematic;

    std::string line;
    while (std::getline(input, line))
    {
        schematic.push_back(std::move(line));
    }

    std::vector<std::pair<int, std::pair<point, point>>> rectangles;

    std::regex re{R"(\d+)"};
    for (int i{0}; i != schematic.size(); ++i)
    {
        std::sregex_iterator it(schematic[i].cbegin(), schematic[i].cend(), re),
            it_end;
        for (; it != it_end; ++it)
        {
            auto const& match = *it;
            auto const number{std::stoi(match.str())};
            auto position{static_cast<int>(it->position())};
            auto length{static_cast<int>(it->length())};

            if (auto box{
                    has_gear_ratio_adjecent(schematic, i, position, length)})
            {
                rectangles.emplace_back(number, std::move(*box));
            }
        }
    }

    int sum{};
    for (int i{0}; i != rectangles.size(); ++i)
    {
        auto first_points = generate_points(rectangles[i].second.first,
            rectangles[i].second.second);
        for (int j{i + 1}; j != rectangles.size(); ++j)
        {
            auto second_points = generate_points(rectangles[j].second.first,
                rectangles[j].second.second);

            std::vector<point> intersect;
            std::set_intersection(first_points.cbegin(),
                first_points.cend(),
                second_points.cbegin(),
                second_points.cend(),
                std::back_inserter(intersect));

            auto const with_gear{std::any_of(intersect.cbegin(),
                intersect.cend(),
                [&schematic](auto const& v)
                { return schematic[v.x][v.y] == '*'; })};

            if (with_gear)
            {
                sum += rectangles[i].first * rectangles[j].first;
            }
        }
    }
    std::cout << sum << '\n';
}
