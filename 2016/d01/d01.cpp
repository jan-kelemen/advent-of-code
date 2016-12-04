#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "Direction.h"
#include "Heading.h"

using command_t = std::pair<Direction, int>;

std::vector<command_t> read_commands(char const* filename);

int process_commands(std::vector<command_t> const& commands);

int main() {
    auto constexpr input_filename = "input.txt";
    auto commands = read_commands(input_filename);
    auto distance = process_commands(commands);

    std::cout << distance << '\n';
    system("pause");
}

std::vector<std::pair<Direction, int>> read_commands(char const* filename) {
    auto constexpr regex_str = "(L|R)(\\d+),?";

    auto rv = std::vector<command_t>();
    std::ifstream input_file(filename);

    auto regex = std::regex(regex_str);

    std::string val;
    while (input_file >> val) {
        std::smatch match;
        if (std::regex_match(val, match, regex)) {
            auto direction = match[1] == "L" ? Direction::Left : Direction::Right;
            auto distance = std::stoi(match[2]);

            rv.emplace_back(direction, distance);
        }        
    }

    return rv;
}

int process_commands(std::vector<command_t> const& commands) {
    auto map = std::unordered_map<Heading, int>{
        {Heading::North, 0}, {Heading::South, 0}, {Heading::East, 0}, {Heading::West, 0}
    };

    auto heading = Heading::North;
    for (auto const& command : commands) {
        heading = rotate(heading, command.first);
        map[heading] += command.second;
    }

    auto horizontal_distance = std::abs(map[Heading::East] - map[Heading::West]);
    auto vertical_distance = std::abs(map[Heading::North] - map[Heading::South]);

    return horizontal_distance + vertical_distance;
}
