#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

class node
{
public:
    node(std::string name, int weight, node* parent = nullptr);

    void add_parent(node* parent);

    node* add_child(std::string, std::unique_ptr<node>&& child = nullptr);

    int weight() const;

    node* parent();

    std::string const& name() const;
private:
    std::string name_;
    int weight_;
    node* parent_;
    std::map<std::string, std::unique_ptr<node>> children_;
};