#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

int main()
{
    std::vector<std::string> inputs{ std::istream_iterator<std::string>(std::cin), {} };
    std::string last_inner;
    std::vector<std::string>::const_iterator it = std::find_first_of(
        inputs.cbegin(), inputs.cend(),
        inputs.cbegin(), inputs.cend(),
        [&last_inner](std::string const& rhs, std::string const& lhs) mutable 
    {
        int diff_count = std::transform_reduce(
            rhs.cbegin(), rhs.cend(),
            lhs.cbegin(),
            0,
            [](int count, bool diff) { return count += diff; },
            [](char c1, char c2) { return c1 != c2; });
        if (diff_count == 1) { last_inner = lhs; return true; }
        return false;
    });
    last_inner.erase(std::mismatch(last_inner.cbegin(), last_inner.cend(), it->cbegin()).first);
    std::cout << last_inner << '\n';
}
