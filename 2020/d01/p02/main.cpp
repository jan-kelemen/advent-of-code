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

    for (iter it1 = std::cbegin(expenses); it1 != std::cend(expenses) - 2;
         ++it1)
    {
        for (iter it2 = it1 + 1; it2 != std::cend(expenses) - 1; ++it2)
        {
            iter matches_sum = std::find_if(it2 + 1,
                std::cend(expenses),
                [i = *it1, j = *it2](int k) { return i + j + k == 2020; });
            if (matches_sum != std::cend(expenses))
            {
                std::cout << *it1 * *it2 * *matches_sum << '\n';
                return EXIT_SUCCESS;
            }
        }
    }
}
