#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
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

    std::size_t tree_count = 0;
    for (std::size_t i = 1; i != trees.size(); ++i)
    {
        std::size_t const effective_index = (i * 3) % trees[i].size();
        tree_count += trees[i][effective_index];
    }
    std::cout << tree_count << '\n';
}
