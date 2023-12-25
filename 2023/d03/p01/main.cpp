#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

bool has_symbol_adjecent(std::vector<std::string> const& schematic,
    int const row,
    int const position,
    int const length)
{
    int const upper_row{std::max(row - 1, 0)};
    int const lower_row{std::min(row + 2, static_cast<int>(schematic.size()))};
    int const left_column{std::max(position - 1, 0)};
    int const right_column{std::min(position + 1 + length,
        static_cast<int>(schematic[row].size()))};

    for (int i{upper_row}; i != lower_row; ++i)
    {
        for (int j{left_column}; j != right_column; ++j)
        {
            auto const chr{schematic[i][j]};
            if (chr != '.' && !isdigit(chr))
            {
                return true;
            }
        }
    }
    return false;
}

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input{argv[1]};

    std::vector<std::string> schematic;

    std::string line;
    while (std::getline(input, line))
    {
        schematic.push_back(std::move(line));
    }

    int sum{};
    std::regex re{R"(\d+)"};
    for (int i{0}; i != schematic.size(); ++i)
    {
        std::sregex_iterator it(schematic[i].cbegin(), schematic[i].cend(), re),
            it_end;
        for (; it != it_end; ++it)
        {
            auto const& match = *it;
            auto const number{std::stoi(match.str())};
            auto position{static_cast<int>(it->position())};
            auto length{static_cast<int>(it->length())};

            if (has_symbol_adjecent(schematic, i, position, length))
            {
                sum += std::stoi(match.str());
            }
        }
    }

    std::cout << sum << '\n';
}
