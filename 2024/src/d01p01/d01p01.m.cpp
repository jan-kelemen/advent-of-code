#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    std::vector<uint64_t> left_list;
    std::vector<uint64_t> right_list;

    uint64_t left, right;
    while (input >> left >> right)
    {
        left_list.push_back(left);
        right_list.push_back(right);
    }
    assert(left_list.size() == right_list.size());

    std::ranges::sort(left_list);
    std::ranges::sort(right_list);

    uint64_t distance{};
    for (size_t i{}; i != left_list.size(); ++i)
    {
        auto const l{left_list[i]};
        auto const r{right_list[i]};

        distance += std::cmp_less(l, r) ? r - l : l - r;
    }

    std::cout << distance << '\n';
}
