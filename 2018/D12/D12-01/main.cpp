#include <algorithm>
#include <deque>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

class condition
{
public:
    condition() = default;
    condition(std::string pattern, char result) : pattern{ std::move(pattern) }, result{ result } {}
    condition(condition const&) = default;
    condition(condition&&) = default;

    template<typename RandIt>
    [[nodiscard]] std::vector<std::size_t> matches(RandIt begin, RandIt end) const noexcept
    {
        static_assert(std::is_base_of_v<std::random_access_iterator_tag, typename std::iterator_traits<RandIt>::iterator_category>);
        static_assert(std::is_same_v<std::remove_cv_t<typename std::iterator_traits<RandIt>::value_type>, char>);
        
        auto rv = std::vector<std::size_t>{};

        for (auto index = std::size_t{ 0 }; begin != end - pattern.size(); ++begin, ++index)
            if (std::equal(begin, begin + pattern.size(), pattern.cbegin(), pattern.cend()))
                rv.push_back(index + pattern.size() / 2);

        return rv;
    }

    [[nodiscard]] char pot() const noexcept { return result; }

    condition& operator=(condition const&) = default;
    condition& operator=(condition&&) = default;
    ~condition() = default;
private:
    std::string pattern{};
    char result{};
};

class pots
{
public:
    using const_iterator = std::string::const_iterator;

    pots() = default;
    pots(pots const&) = default;
    pots(pots&&) = default;
    template<typename InputIt>
    pots(InputIt begin, InputIt end) : sequence{ begin, end }, marks(sequence.size(), 0)
    {
        static_assert(std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>);
        static_assert(std::is_same_v<std::remove_cv_t<typename std::iterator_traits<InputIt>::value_type>, char>);
        std::iota(marks.begin(), marks.end(), 0);
    }

    const_iterator begin() const { return sequence.find_first_of('#') + sequence.cbegin() - 1; }
    const_iterator cbegin() const { return begin(); }
    const_iterator end() const { return sequence.find_last_of('#') + sequence.cbegin() + 2; }
    const_iterator cend() const { return end(); }

    template<typename InputIt>
    int run_generation(InputIt begin, InputIt end)
    {
        static_assert(std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>);
        static_assert(std::is_same_v<std::remove_cv_t<typename std::iterator_traits<InputIt>::value_type>, condition>);

        expand();
        std::vector<std::size_t> const matches = collect_matches(sequence, begin, end);
        map_matches(matches);

        return std::transform_reduce(
            sequence.cbegin(), sequence.cend(),
            marks.cbegin(),
            0,
            [](int s, std::pair<char, int> const& p) { return p.first == '#' ? s + p.second : s; },
            [](char c, int i) { return std::make_pair(c, i); });
    }

    pots& operator=(pots const&) = default;
    pots& operator=(pots&&) = default;
    ~pots() = default;
private:
    void expand()
    {
        while(sequence.find_first_of('#') <= 5)
        {
            sequence.insert(sequence.begin(), '.');
            marks.push_front(marks[0] - 1);
        }
        while(sequence.find_last_of('#') != sequence.size() - 6)
        {
            sequence.push_back('.');
            marks.push_back(*marks.crbegin() + 1);
        }
    }

    template<typename InputIt>
    std::vector<std::size_t> collect_matches(std::string const& sequence, InputIt begin, InputIt end) const
    {
        auto rv = std::vector<std::size_t>{};
        for (; begin != end; ++begin)
        {
            condition condition = *begin;
            if(condition.pot() == '#')
            {
                std::vector<std::size_t> matches = begin->matches(sequence.cbegin(), sequence.cend());
                std::copy(matches.cbegin(), matches.cend(), std::back_inserter(rv));
            }            
        }
        return rv;
    }

    void map_matches(std::vector<std::size_t> const& matches)
    {
        auto temp = sequence;
        std::replace(sequence.begin(), sequence.end(), '#', '.');
        for (auto const& match : matches)
            sequence[match] = '#';
    }
    std::string sequence;
    std::deque<int> marks;
};

[[nodiscard]] std::pair<pots, std::vector<condition>> read_data(std::istream& stream)
{
    using namespace std::string_literals;
    auto line = ""s;
    std::getline(stream, line);
    auto p = pots{ line.find_last_of(' ') + line.cbegin() + 1, line.cend() };
    std::getline(stream, line);

    auto c = std::vector<condition>{};
    while (std::getline(stream, line))
        c.emplace_back(line.substr(0, 5), *line.rbegin());

    return { p, c };
}

int main()
{
    using namespace std::string_literals;
    auto[pots, conditions] = read_data(std::cin);
    for (auto i = 0; i < 19; ++i)
    {
        pots.run_generation(conditions.cbegin(), conditions.cend());
    }
    std::cout << pots.run_generation(conditions.cbegin(), conditions.cend());
}