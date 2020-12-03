#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

using tree_matrix_t = std::vector<std::vector<bool>>;

int main(int argc, char** argv)
{
    std::ifstream input = std::ifstream(argv[1]);

    tree_matrix_t trees;

    std::string line;
    while (std::getline(input, line))
    {
        std::vector<bool> single_row;
        std::transform(std::cbegin(line),
            std::cend(line),
            std::back_inserter(single_row),
            [](char c) { return c == '#'; });
        trees.push_back(std::move(single_row));
    }

    std::array<std::pair<int, int>, 5> slopes = {std::make_pair(1, 1),
        std::make_pair(3, 1),
        std::make_pair(5, 1),
        std::make_pair(7, 1),
        std::make_pair(1, 2)};

    std::vector<std::uint64_t> slope_tree_count(5, 0);
    for (std::size_t i = 0; i != slopes.size(); ++i)
    {
        for (std::size_t y = 0; y * slopes[i].second < trees.size(); ++y)
        {
            std::size_t const effective_index =
                (y * slopes[i].first) % trees[y].size();
            slope_tree_count[i] += trees[y * slopes[i].second][effective_index];
        }
    }

    std::cout << std::accumulate(std::cbegin(slope_tree_count),
                     std::cend(slope_tree_count),
                     std::uint64_t(1),
                     std::multiplies<>())
              << '\n';
}
