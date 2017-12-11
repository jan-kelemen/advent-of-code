#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using name_t = std::string;
using weight_t = int;
using children_list_t = std::vector<std::string>;
using node_data_t = std::tuple<weight_t, name_t, children_list_t>;

std::map<name_t, node_data_t> read_tree(std::istream& stream);

std::string node_without_parent(std::map<name_t, node_data_t> const& tree);

weight_t find_unbalanced_node(std::map<name_t, node_data_t> const& tree, name_t const& root);

int main(int argc, char** argv)
{
    auto tree = read_tree(std::cin);
    auto root = node_without_parent(tree);
    auto unbalanced = find_unbalanced_node(tree, root);
    std::cout << unbalanced << '\n';
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
    for(auto const&[name, data] : tree)
    {
        if(!std::get<1>(data).size())
        {
            return name;
        }
    }
    return {};
}

void calculate_weights(std::map<name_t, node_data_t> const& tree, name_t const& node_name, std::map<name_t, weight_t>& weights)
{
    auto const& root = tree.at(node_name);

    auto sum = std::get<0>(root);
    for(auto const& child : std::get<2>(root))
    {
        if(!weights.count(child))
        {
            calculate_weights(tree, child, weights);
        }
        sum += weights[child];
    }
    weights[node_name] = sum;
}

std::map<name_t, weight_t> calculate_weights(std::map<name_t, node_data_t> const& tree, name_t const& node_name)
{
    std::map<name_t, weight_t> rv;
    calculate_weights(tree, node_name, rv);
    return rv;
}

std::map<int, std::vector<name_t const*>> get_child_weights(children_list_t const& children, std::map<name_t, int> const& weights)
{
    std::map<int, std::vector<name_t const*>> rv;
    std::for_each(children.begin(), children.end(), [&rv, &weights](auto const& name) { rv[weights.at(name)].push_back(std::addressof(name)); });
    return rv;
}

//don't even ask
weight_t find_unbalanced_node(std::map<name_t, node_data_t> const& tree, name_t const& root)
{
    auto current_node = root;
    auto weights = calculate_weights(tree, root);

    auto current_node_child_weights = get_child_weights(std::get<2>(tree.at(current_node)), weights);
    while(current_node_child_weights.size() != 1)
    {
        auto difference = 0;
        
        for(auto const&[weight, names] : current_node_child_weights)
        {
            if(names.size() == 1)
            {
                current_node = *(names[0]);
                difference -= weight;
            }
            else
            {
                difference += weight;
            }
        }

        auto next_child_weights = get_child_weights(std::get<2>(tree.at(current_node)), weights);
        if(next_child_weights.size() == 2)
        {
            current_node_child_weights = std::move(next_child_weights);
        }
        else
        {
            return std::get<0>(tree.at(current_node)) + difference;
        }
    }

    return 0;
}

