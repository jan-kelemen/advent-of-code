#include "node.h"

node::node(std::string name, int weight, node* parent)
    : name_(std::move(name)), weight_(std::move(weight)), parent_(std::move(parent))
{
}
