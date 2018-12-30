#include <algorithm>
#include <cstdio>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <iterator>

[[nodiscard]] int convergence_index(
    std::vector<int> x_points, std::vector<int> const& x_velocity,
    std::vector<int> y_points, std::vector<int> const& y_velocity)
{
    auto const calc_area = [](std::pair<int64_t, int64_t> const& x, std::pair<int64_t, int64_t> const& y)
    {
        return (x.second - x.first) * (y.second - y.first);
    };

    auto index = std::numeric_limits<int>::max();
    auto min_area = std::numeric_limits<int64_t>::max();
    for (auto i = 0; i < 11000; ++i)
    {
        auto const x = std::minmax_element(x_points.cbegin(), x_points.cend());
        auto const y = std::minmax_element(y_points.cbegin(), y_points.cend());
        auto const area = calc_area({ *x.second, *x.first }, { *y.second, *y.first });
        if (area < min_area)
        {
            index = i;
            min_area = area;
        }

        std::transform(x_points.cbegin(), x_points.cend(), x_velocity.cbegin(), x_points.begin(), std::plus<>{});
        std::transform(y_points.cbegin(), y_points.cend(), y_velocity.cbegin(), y_points.begin(), std::plus<>{});
    }

    return index;
}

std::set<std::pair<int, int>> combine(std::vector<int> const& x_points, std::vector<int> const& y_points)
{
    auto rv = std::set<std::pair<int, int>>{};
    auto const combine = [](int const& x, int const& y) { return std::make_pair(x, y); };
    std::transform(x_points.cbegin(), x_points.cend(), y_points.cbegin(), std::inserter(rv, rv.begin()), combine);
    return rv;
}

void fast_forward(
    std::vector<int>& x_points, std::vector<int> const& x_velocity,
    std::vector<int>& y_points, std::vector<int> const& y_velocity,
    int const index)
{
    auto const index_aware_plus = [index](int const& x, int const& v) { return x + index * v; };
    std::transform(x_points.cbegin(), x_points.cend(), x_velocity.cbegin(), x_points.begin(), index_aware_plus);
    std::transform(y_points.cbegin(), y_points.cend(), y_velocity.cbegin(), y_points.begin(), index_aware_plus);
}

std::pair<int, int> find_binds(std::vector<int> const& points)
{
    auto const its = std::minmax_element(points.cbegin(), points.cend());
    return { *its.first, *its.second };
}

std::pair<std::pair<int, int>, std::pair<int, int>> find_bounding_box(std::vector<int> const& x_points, std::vector<int> const& y_points)
{
    return { find_binds(x_points), find_binds(y_points) };
}

void show_sky(std::ostream& stream, std::vector<int> const& x_points, std::vector<int> const& y_points)
{
    auto const&[x_bound, y_bound] = find_bounding_box(x_points, y_points);
    std::set<std::pair<int, int>> const points = combine(x_points, y_points);
    for (auto j = y_bound.first; j != y_bound.second + 1; ++j)
    {
        for (auto i = x_bound.first; i != x_bound.second + 1; ++i)
            stream << (points.count(std::make_pair(i, j)) ? '#' : '.');
        stream << '\n';
    }
}

int main()
{
    using namespace std::string_literals;

    auto x_points = std::vector<int>{};
    auto y_points = std::vector<int>{};
    auto x_velocity = std::vector<int>{};
    auto y_velocity = std::vector<int>{};

    auto line = ""s;
    while (std::getline(std::cin, line))
    {
        auto point_x = 0, point_y = 0, velocity_x = 0, velocity_y = 0;
        std::sscanf(line.c_str(), "position=<%d, %d> velocity=<%d, %d>", &point_x, &point_y, &velocity_x, &velocity_y);
        x_points.push_back(point_x + velocity_x);
        y_points.push_back(point_y + velocity_y);
        x_velocity.push_back(velocity_x);
        y_velocity.push_back(velocity_y);
    }

    int const index = convergence_index(x_points, x_velocity, y_points, y_velocity);
    fast_forward(x_points, x_velocity, y_points, y_velocity, index);
    show_sky(std::cout, x_points, y_points);
}
