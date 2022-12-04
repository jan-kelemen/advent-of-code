#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <utility>

using assignment_t = std::pair<int, int>;

bool is_overlapping(assignment_t const& lhs, assignment_t const& rhs)
{
    return rhs.first >= lhs.first && rhs.first <= lhs.second ||
        rhs.second >= lhs.first && rhs.second <= lhs.second;
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
            is_overlapping(first, second) || is_overlapping(second, first);
    }

    std::cout << overlapping << '\n';
}
