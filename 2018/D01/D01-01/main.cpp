#include <iostream>
#include <iterator>
#include <numeric>

int main()
{
    std::cout << std::accumulate(std::istream_iterator<int>(std::cin), std::istream_iterator<int>(), 0) << '\n';
}
