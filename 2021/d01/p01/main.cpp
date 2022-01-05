#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input = std::ifstream(argv[1]);
    std::vector<int> const depths =
        std::vector<int>(std::istream_iterator<int>(input), {});

    size_t const count = std::count_if(std::cbegin(depths) + 1,
        std::cend(depths),
        [last = depths[0]](int const v) mutable
        {
            bool const rv = last < v;
            last = v;
            return rv;
        });
    std::cout << count << '\n';
}
