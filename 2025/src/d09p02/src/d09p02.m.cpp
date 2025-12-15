#include <fmt/format.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <vector>

enum class color : char
{
    none = '.',
    red = '#',
    green = 'X',
};

struct [[nodiscard]] coord
{
    coord(int i, int j) : original_x{i}, x{i}, original_y{j}, y{j} { }

    size_t id;

    int original_x;
    int x;
    int original_y;
    int y;
};

struct [[nodiscard]] cmp_by_id
{
    [[nodiscard]] static constexpr bool operator()(coord const& lhs,
        coord const& rhs)
    {
        return lhs.id < rhs.id;
    }
};

struct [[nodiscard]] cmp_by_x
{
    [[nodiscard]] static constexpr bool operator()(coord const& lhs,
        coord const& rhs)
    {
        return lhs.x < rhs.x;
    }
};

struct [[nodiscard]] cmp_by_y
{
    [[nodiscard]] static constexpr bool operator()(coord const& lhs,
        coord const& rhs)
    {
        return lhs.y < rhs.y;
    }
};

int main([[maybe_unused]] int const argc, char const* argv[])

{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    static std::regex const points_regex{R"((\d+),(\d+))"};

    std::vector<coord> positions;

    std::string line;
    while (std::getline(input, line))
    {
        std::smatch match;
        [[maybe_unused]] bool const matched{
            std::regex_match(line.cbegin(), line.cend(), match, points_regex)};
        assert(matched);

        auto& added{
            positions.emplace_back(std::stoi(match[1]), std::stoi(match[2]))};
        added.id = positions.size();
    }

    std::ranges::sort(positions, cmp_by_x{});
    for (int i : std::views::iota(0, static_cast<int>(positions.size())))
    {
        positions[i].x = i / 2;
    }
    std::ranges::sort(positions, cmp_by_y{});
    for (int i : std::views::iota(0, static_cast<int>(positions.size())))
    {
        positions[i].y = i / 2;
    }
    std::ranges::sort(positions, cmp_by_id{});

    std::vector<std::vector<color>> grid{positions.size() / 2,
        std::vector<color>{positions.size() / 2, color::none}};
    for (size_t i{}; i != positions.size(); ++i)
    {
        auto const& this_point{positions[i]};
        auto const& next_point{positions[(i + 1) % positions.size()]};
        for (size_t y = std::min(this_point.y, next_point.y);
            y != std::max(this_point.y, next_point.y) + 1;
            ++y)
        {
            for (size_t x = std::min(this_point.x, next_point.x);
                x != std::max(this_point.x, next_point.x) + 1;
                ++x)
            {
                grid[y][x] = color::green;
            }
        }

        grid[this_point.y][this_point.x] = color::red;
        grid[next_point.y][next_point.x] = color::red;
    }

    int64_t area{};
    for (size_t i{}; i != positions.size() - 1; ++i)
    {
        auto const& this_point{positions[i]};
        for (size_t j{i + 1}; j != positions.size(); ++j)
        {
            auto const& next_point{positions[j]};
            auto const local_area{(std::abs(int64_t{this_point.original_x} -
                                       next_point.original_x) +
                                      1) *
                (std::abs(
                     int64_t{this_point.original_y} - next_point.original_y) +
                    1)};
            if (local_area > area)
            {
                bool is_ok{true};
                for (int y{std::min(this_point.y, next_point.y) + 1};
                    y != std::max(this_point.y, next_point.y);
                    ++y)
                {
                    if (!std::ranges::all_of(grid[y] |
                                std::views::drop(
                                    std::min(this_point.x, next_point.x) + 1) |
                                std::views::take(
                                    std::abs(this_point.x - next_point.x)),
                            [](auto c) { return c == color::none; }))
                    {
                        is_ok = false;
                        break;
                    }
                }

                if (is_ok)
                {
                    area = local_area;
                }
            }
        }
    }

    fmt::println("Max: {}", area);
}
