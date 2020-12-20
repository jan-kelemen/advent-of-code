#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int main(int argc, char** argv)
{
    std::ifstream input = std::ifstream(argv[1]);
    std::vector<int> expenses =
        std::vector<int>(std::istream_iterator<int>(input), {});
    using iter = std::vector<int>::const_iterator;

    for (iter it = std::cbegin(expenses); it != std::cend(expenses) - 1; ++it)
    {
        iter matches_sum = std::find_if(it + 1,
            std::cend(expenses),
            [l = *it](int r) { return l + r == 2020; });
        if (matches_sum == std::cend(expenses))
        {
            continue;
        }
        std::cout << *it * *matches_sum << '\n';
    }
}
