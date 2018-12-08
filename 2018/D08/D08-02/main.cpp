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
    if (tree.child_nodes.empty())
        return std::reduce(tree.metadata.cbegin(), tree.metadata.cend());

    return std::reduce(
        tree.metadata.cbegin(), tree.metadata.cend(),
        std::size_t{ 0 },
        [r = tree](std::size_t s, std::size_t const& m) { return (m == 0 || m - 1 >= r.child_nodes.size()) ? s : s + metadata_sum(r.child_nodes[m - 1]); });
}

int main()
{
    auto const tree = *std::istream_iterator<node>{std::cin};
    std::cout << metadata_sum(tree) << '\n';
}