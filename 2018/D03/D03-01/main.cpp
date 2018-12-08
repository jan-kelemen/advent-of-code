#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <utility>
#include <vector>

struct claim
{
    claim() = default;
    claim(int id, std::pair<int, int> const& top_left, std::pair<int, int> const& dimensions);
    claim(claim const&) = default;
    claim(claim&&) = default;

    claim& operator=(claim const&) = default;
    claim& operator=(claim&&) = default;

    int id = {};
    std::pair<int, int> top_left = {};
    std::pair<int, int> bottom_right = {};

    std::vector<std::pair<int, int>> points = {};
};

claim::claim(int id, std::pair<int, int> const& top_left, std::pair<int, int> const& dimensions)
    : id{ id }
    , top_left{ top_left }
    , bottom_right{ top_left.first + dimensions.first, top_left.second + dimensions.second }
{
    points.reserve(dimensions.first * dimensions.second);
    for (auto x = top_left.first; x < bottom_right.first; ++x)
        for (auto y = top_left.second; y < bottom_right.second; ++y)
            points.emplace_back(x, y);
}

std::istream& operator>>(std::istream& stream, claim& claim)
{
    using claim_t = struct claim;

    using namespace std::string_literals;
    auto line = ""s;
    std::getline(stream, line);
    auto id = 0, top_x = 0, top_y = 0, dim_x = 0, dim_y = 0;
    sscanf(line.c_str(), "#%d @ %d,%d: %dx%d", &id, &top_x, &top_y, &dim_x, &dim_y);
    claim = claim_t{ id, std::make_pair(top_x, top_y), std::make_pair(dim_x, dim_y) };
    return stream;
}

std::vector<std::pair<int, int>> intersect(claim const& rhs, claim const& lhs)
{
    auto intersection = std::vector<std::pair<int, int>>{};
    std::set_intersection(
        rhs.points.cbegin(), rhs.points.cend(),
        lhs.points.cbegin(), lhs.points.cend(),
        std::back_inserter(intersection));
    return intersection;
}

int main()
{
    auto claims = std::vector<claim>{};
    std::copy(std::istream_iterator<claim>(std::cin), {}, std::back_inserter(claims));

    auto colliding_points = std::set<std::pair<int, int>>{};
    auto index = 0;
    for (auto it = claims.cbegin(); it != claims.cend() - 1; ++it)
    {
        for (auto inner_it = it + 1; inner_it != claims.cend(); ++inner_it)
        {
            std::vector<std::pair<int, int>> intersection = intersect(*it, *inner_it);
            colliding_points.insert(intersection.cbegin(), intersection.cend());
        }
    }
    std::cout << colliding_points.size() << '\n';
}