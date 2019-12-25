#include <numeric>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

int main()
{
  std::ifstream stream{ "input.txt" };
  std::cout << std::accumulate(
    std::istream_iterator<int64_t>{ stream }, {},
    int64_t{ 0 },
    [](int64_t const sum, int64_t const v) { return sum + v / 3 - 2; }) << '\n';
}