#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

int main([[maybe_unused]] int const argc, char const* argv[])
{
    assert(argc >= 2);
    std::ifstream input{argv[1]};

    std::vector<uint64_t> left_list;
    std::map<uint64_t, size_t> right_list;

    uint64_t left, right;
    while (input >> left >> right)
    {
        left_list.push_back(left);
        ++right_list[right];
    }

    uint64_t similarity{};
    for (uint64_t const l : left_list)
    {
        auto const r{right_list[l]};

        similarity += l * r;
    }

    std::cout << similarity << '\n';
}
