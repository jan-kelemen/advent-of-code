#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <string>

int main()
{
    std::pair<int, int> final_count = std::transform_reduce(
        std::istream_iterator<std::string>(std::cin), {},
        std::make_pair(0, 0),
        [](std::pair<int, int> p, std::pair<int, int> const& b)
    {
        p.first += b.first;
        p.second += b.second;
        return p;
    },
        [](std::string const& id)
    {
        std::map<char, int> letter_counts = std::reduce(
            id.cbegin(), id.cend(), 
            std::map<char, int>(), 
            [](std::map<char, int> lc, char c) { ++lc[c]; return lc; });
        return std::reduce(
            letter_counts.cbegin(), letter_counts.cend(),
            std::make_pair(0, 0),
            [](std::pair<int, int> r, std::pair<char, int> const& p)
        {
            r.first |= p.second == 2; 
            r.second |= p.second == 3; 
            return r;
        });
    });
    std::cout << final_count.first * final_count.second << '\n';
}
