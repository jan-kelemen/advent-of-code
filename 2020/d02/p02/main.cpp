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

        std::size_t const min = std::stoi(m[1].str()) - 1;
        std::size_t const max = std::stoi(m[2].str()) - 1;
        char const character = m[3].str()[0];
        std::string const password = m[4].str();

        auto const either = [](std::string const& line,
                                std::size_t const index1,
                                std::size_t const index2,
                                char const character) {
            return line[index1] == character && line[index2] != character ||
                line[index1] != character && line[index2] == character;
        };

        valid_count += either(password, min, max, character);
    }

    std::cout << valid_count << '\n';
}
