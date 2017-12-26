#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using name_t = std::string;
using weight_t = int;
using children_list_t = std::vector<std::string>;
using node_data_t = std::tuple<weight_t, name_t, children_list_t>;

std::map<name_t, node_data_t> read_tree(std::istream& stream);

std::string node_without_parent(std::map<name_t, node_data_t> const& tree);

int main(int argc, char** argv)
{
    auto tree = read_tree(std::cin);
    auto root = node_without_parent(tree);
    std::cout << root << '\n';
}

std::map<name_t, node_data_t> read_tree(std::istream& stream)
{
    auto tree = std::map<name_t, node_data_t>();

    std::string line;
    while(std::getline(stream, line))
    {
        std::string clean_line;
        std::copy_if(line.begin(), line.end(), std::back_inserter(clean_line), [](char c) {
            return c != '(' && c != ')' && c != '-' && c != '>' && c != ','; 
        });

        auto strstream = std::istringstream(clean_line);

        name_t name;
        strstream >> name;

        auto& node = tree[name];
        strstream >> std::get<0>(node);

        name_t child_name;
        while(strstream >> child_name)
        {
            auto& child = tree[child_name];
            std::get<1>(child) = name;
            std::get<2>(node).push_back(child_name);
        }
    }

    return tree;
}

std::string node_without_parent(std::map<name_t, node_data_t> const& tree)
{
    for(auto const& [name, data] : tree)
    {
        if(!std::get<1>(data).size())
        {
            return name;
        }
    }
    return {};
}
