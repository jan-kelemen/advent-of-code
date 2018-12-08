#include <algorithm>
#include <array>
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

    [[nodiscard]] constexpr char source() const noexcept { return data.first; }
    [[nodiscard]] constexpr char target() const noexcept { return data.second; }

private:
    std::pair<char, char> data;

    friend bool operator==(dependency const&, dependency const&) noexcept;
    friend bool operator<(dependency const&, dependency const&) noexcept;
};

[[nodiscard]] bool operator==(dependency const& lhs, dependency const& rhs) noexcept { return lhs.data == rhs.data; }
[[nodiscard]] bool operator!=(dependency const& lhs, dependency const& rhs) noexcept { return !(lhs == rhs); }
[[nodiscard]] bool operator<(dependency const& lhs, dependency const& rhs) noexcept { return lhs.data < rhs.data; }
[[nodiscard]] bool operator>(dependency const& lhs, dependency const& rhs) noexcept { return rhs < lhs; }
[[nodiscard]] bool operator<=(dependency const& lhs, dependency const& rhs) noexcept { return !(rhs < lhs); }
[[nodiscard]] bool operator>=(dependency const& lhs, dependency const& rhs) noexcept { return !(lhs < rhs); }

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

template<std::size_t N>
struct availability_checker
{
    explicit availability_checker(std::array<std::pair<char, int8_t>, N> const& w)
        : workers{ w }
    {
    }

    [[nodiscard]] bool operator()(std::pair<char, std::set<char>> const& p) const noexcept
    {
        return p.second.empty() &&
            std::any_of(workers.cbegin(), workers.cend(), [](std::pair<char, int8_t> const& r) { return r.first == '\0'; }) &&
            std::all_of(workers.cbegin(), workers.cend(), [&t = p.first](std::pair<char, int8_t> const& r) { return r.first != t; });;
    }

    std::array<std::pair<char, int8_t>, N> const& workers;
};

template<typename InputIt>
[[nodiscard]] int run_dependent_tree(InputIt begin, InputIt end)
{
    static_assert(std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>);
    static_assert(std::is_same_v<std::remove_cv_t<typename std::iterator_traits<InputIt>::value_type>, dependency>);

    auto tree = std::map<char, std::set<char>>{};
    for (; begin != end; ++begin)
    {
        tree[begin->target()].insert(begin->source());
        tree[begin->source()];
    }

    auto seconds = 0;
    auto workers = std::array<std::pair<char, int8_t>, 5>{};
    auto available_workers = workers.size();
    while (!tree.empty())
    {
        seconds++;
        while (true)
        {
            auto const executable = std::find_if(tree.cbegin(), tree.cend(), availability_checker{ workers });
            if (executable == tree.cend() || available_workers == 0)
                break;

            char const task = executable->first;

            auto empty = std::find_if(
                workers.begin(), workers.end(),
                [](std::pair<char, int8_t> const& p) { return p.first == '\0'; });
            *empty = std::make_pair(task, int8_t{ 61 + task - 'A' });
            --available_workers;
        }

        for (auto& p : workers)
        {
            if(p.second > 0)
                --p.second;

            if(p.second == 0)
            {
                tree.erase(p.first);
                for (auto& task : tree)
                    task.second.erase(p.first);
                p.first = '\0';
                ++available_workers;
            }
        }
    }

    return seconds;
}

int main()
{
    std::cout << run_dependent_tree(std::istream_iterator<dependency>{std::cin}, {}) << '\n';
}
