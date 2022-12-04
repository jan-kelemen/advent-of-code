#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <utility>

using assignment_t = std::pair<int, int>;

bool is_contained(assignment_t const& lhs, assignment_t const& rhs)
{
    return lhs.first <= rhs.first && lhs.second >= rhs.second;
}

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input {argv[1]};

    std::regex const assignments {R"((\d+)-(\d+),(\d+)-(\d+))"};

    int64_t overlapping {};

    std::string line;
    while (std::getline(input, line))
    {
        std::smatch m;
        std::regex_match(line, m, assignments);

        assignment_t const first {std::stoi(m[1].str()), std::stoi(m[2].str())};
        assignment_t const second {std::stoi(m[3].str()),
            std::stoi(m[4].str())};

        overlapping +=
            is_contained(first, second) || is_contained(second, first);
    }

    std::cout << overlapping << '\n';
}
