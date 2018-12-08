#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

struct node
{
    std::vector<node> child_nodes{};
    std::vector<std::size_t> metadata{};
};

std::istream& operator>>(std::istream& stream, node& n)
{
    auto children = std::size_t{ 0 }, metadata = std::size_t{ 0 };
    stream >> children >> metadata;

    n.child_nodes.resize(children);
    //std::copy_n results in stack overflow, sorry :(
    for (auto i = std::size_t{ 0 }; i != children; ++i)
        stream >> n.child_nodes[i];

    n.metadata.reserve(metadata);
    std::copy_n(std::istream_iterator<std::size_t>{stream}, metadata, std::back_inserter(n.metadata));

    return stream;
}

[[nodiscard]] std::size_t metadata_sum(node const& tree) noexcept
{
    auto const sum = std::reduce(tree.metadata.cbegin(), tree.metadata.cend());
    return std::reduce(tree.child_nodes.begin(), tree.child_nodes.end(), sum, [](std::size_t s, node const& n) { return s + metadata_sum(n); });
}

int main()
{
    auto const tree = *std::istream_iterator<node>{std::cin};
    std::cout << metadata_sum(tree) << '\n';
}