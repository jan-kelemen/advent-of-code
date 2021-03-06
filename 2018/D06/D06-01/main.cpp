#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>
#include <map>
#include <optional>

class point
{
public:
    explicit point(int x = 0, int y = 0) noexcept : data{ x, y } {}

    [[nodiscard]] constexpr int x() const { return data.first; }
    [[nodiscard]] constexpr int y() const { return data.second; }

private:
    std::pair<int, int> data;

    friend bool operator==(point const&, point const&);
    friend bool operator<(point const&, point const&);
};

[[nodiscard]] bool operator==(point const& lhs, point const& rhs) { return lhs.data == rhs.data; }
[[nodiscard]] bool operator!=(point const& lhs, point const& rhs) { return !(lhs == rhs); }
[[nodiscard]] bool operator<(point const& lhs, point const& rhs) { return lhs.data < rhs.data; }
[[nodiscard]] bool operator>(point const& lhs, point const& rhs) { return rhs < lhs; }
[[nodiscard]] bool operator<=(point const& lhs, point const& rhs) { return !(rhs < lhs); }
[[nodiscard]] bool operator>=(point const& lhs, point const& rhs) { return !(lhs < rhs); }

std::istream& operator>>(std::istream& stream, point& p)
{
    using namespace std::string_literals;
    auto line = ""s;
    std::getline(stream, line);
    auto x = 0, y = 0;
    sscanf(line.c_str(), "%d, %d", &x, &y);
    p = point{ x, y };
    return stream;
}

template<typename ForwardIt>
[[nodiscard]] std::pair<point, point> find_bounding_box(ForwardIt begin, ForwardIt end)
{
    static_assert(std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<ForwardIt>::iterator_category>);
    static_assert(std::is_same_v<std::remove_cv_t<typename std::iterator_traits<ForwardIt>::value_type>, point>);

    std::pair<ForwardIt, ForwardIt> res = std::minmax_element(begin, end);
    std::pair<ForwardIt, ForwardIt> res_y = std::minmax_element(begin, end, [](point const& l, point const& r) { return l.y() < r.y(); });
    return { point{res.first->x(), res_y.first->y()}, point{res.second->x(), res_y.second->y()} };
}

int main()
{
    auto points = std::vector<point>{ std::istream_iterator<point>{std::cin}, {} };
    auto bounding_box = find_bounding_box(points.cbegin(), points.cend());

    auto infinite_points = std::set<point>{};
    auto point_count = std::map<point, int>{};
    for (auto x = bounding_box.first.x(); x <= bounding_box.second.x(); ++x)
    {
        for (auto y = bounding_box.first.y(); y <= bounding_box.second.y(); ++y)
        {
            auto distances = std::map<int, std::vector<std::size_t>>{};
            for (auto i = std::size_t{}; i != points.size(); ++i)
            {
                distances[std::abs(x - points[i].x()) + std::abs(y - points[i].y())].push_back(i);
            }

            using element_t = std::pair<int, std::vector<std::size_t>>;
            element_t const& closest = *std::min_element(
                distances.cbegin(), distances.cend(),
                [](element_t const& l, element_t const& r) { return l.first < r.first; });

            auto const& p = points[*closest.second.begin()];
            if (closest.second.size() == 1)
            {
                ++point_count[p];
                if (x == bounding_box.first.x() ||
                    x == bounding_box.second.x() ||
                    y == bounding_box.first.y() ||
                    y == bounding_box.second.y())
                {
                    infinite_points.insert(p);
                }
            }
        }
    }

    using element_t = std::pair<point, int>;
    auto const max = std::max_element(
        point_count.cbegin(), point_count.cend(),
        [&infinite_points](element_t const& l, element_t const& r) { return infinite_points.count(r.first) == 0 && l.second < r.second; });

    std::cout << max->second << '\n';
}
