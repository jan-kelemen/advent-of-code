#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <string>

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input {argv[1]};

    std::string const datastream {std::istream_iterator<char> {input}, {}};

    for (auto begin = std::cbegin(datastream),
              end = std::cbegin(datastream) + 4;
         end != std::cend(datastream);
         ++begin, ++end)
    {
        std::set<char> unique_chars {begin, end};
        if (unique_chars.size() ==
            static_cast<size_t>(std::distance(begin, end)))
        {
            std::cout << std::distance(std::cbegin(datastream), end) << '\n';
            break;
        }
    }
}
