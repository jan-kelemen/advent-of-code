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

    size_t count = 0;
    for (size_t i = 0; i != depths.size() - 3; ++i)
    {
        if (depths[i + 3] > depths[i])
        {
            ++count;
        }
    }
    std::cout << count << '\n';
}
