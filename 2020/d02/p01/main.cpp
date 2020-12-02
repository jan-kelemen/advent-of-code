#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

int main(int argc, char** argv)
{
    std::ifstream input = std::ifstream(argv[1]);

    std::regex const password_policy_regex =
        std::regex(R"((\d+)-(\d+) ([a-z]): ([a-z]+))");

    std::size_t valid_count = 0;
    std::string line;
    while (std::getline(input, line))
    {
        std::smatch m;
        std::regex_match(line, m, password_policy_regex);

        std::size_t const min = std::stoi(m[1].str());
        std::size_t const max = std::stoi(m[2].str());
        char const character = m[3].str()[0];
        std::string const password = m[4].str();

        std::size_t const total_count =
            std::count(std::cbegin(password), std::cend(password), character);

        valid_count += std::clamp(total_count, min, max) == total_count;
    }

    std::cout << valid_count << '\n';
}
