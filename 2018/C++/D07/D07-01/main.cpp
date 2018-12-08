#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <numeric>

class dependency
{
public:
    explicit dependency(char source = 0, char target = 0) noexcept : data{ source, target } {}

    [[nodiscard]] constexpr char source() const { return data.first; }
    [[nodiscard]] constexpr char target() const { return data.second; }

private:
    std::pair<char, char> data;

    friend bool operator==(dependency const&, dependency const&);
    friend bool operator<(dependency const&, dependency const&);
};

[[nodiscard]] bool operator==(dependency const& lhs, dependency const& rhs) { return lhs.data == rhs.data; }
[[nodiscard]] bool operator!=(dependency const& lhs, dependency const& rhs) { return !(lhs == rhs); }
[[nodiscard]] bool operator<(dependency const& lhs, dependency const& rhs) { return lhs.data < rhs.data; }
[[nodiscard]] bool operator>(dependency const& lhs, dependency const& rhs) { return rhs < lhs; }
[[nodiscard]] bool operator<=(dependency const& lhs, dependency const& rhs) { return !(rhs < lhs); }
[[nodiscard]] bool operator>=(dependency const& lhs, dependency const& rhs) { return !(lhs < rhs); }

std::istream& operator>>(std::istream& stream, dependency& d)
{
    using namespace std::string_literals;
    auto line = ""s;
    std::getline(stream, line);
    auto target = '\0', source = '\0';
    sscanf(line.c_str(), "Step %c must be finished before step %c can begin.", &target, &source);
    d = dependency{ target, source };
    return stream;
}

template<typename InputIt>
[[nodiscard]] std::vector<char> run_dependent_tree(InputIt begin, InputIt end)
{
    static_assert(std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>);
    static_assert(std::is_same_v<std::remove_cv_t<typename std::iterator_traits<InputIt>::value_type>, dependency>);

    auto rv = std::vector<char>{};
    auto tree = std::map<char, std::set<char>>{};

    for(; begin != end; ++begin)
    {
        tree[begin->target()].insert(begin->source());
        tree[begin->source()];
    }

    while(!tree.empty())
    {
        auto const executable = std::find_if(
            tree.cbegin(), tree.cend(),
            [](std::pair<char, std::set<char>> const& p) { return p.second.empty(); });
        char const task = executable->first;

        rv.push_back(task);

        tree.erase(task);
        for(auto& p : tree)
            p.second.erase(task);
    }

    return rv;
}

int main()
{
    using namespace std::string_literals;
    std::vector<char> const order = run_dependent_tree(std::istream_iterator<dependency>{std::cin}, {});
    std::string const result = std::accumulate(order.cbegin(), order.cend(), ""s);
    std::cout << result << '\n';
}
