#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <set>
#include <vector>

#include "Direction.h"
#include "Heading.h"

using command_t = std::pair<Direction, int>;
using coordinate_t = std::pair<int, int>;

std::vector<command_t> read_commands(char const* filename);

std::pair<int, int> process_commands(std::vector<command_t> const& commands);

int main() {
    auto constexpr input_filename = "input.txt";
    auto commands = read_commands(input_filename);
    auto result = process_commands(commands);

    std::cout << result.first << ", " << result.second << '\n';
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

int calculate_distance(coordinate_t const& location) {
    return std::abs(location.first) + std::abs(location.second);
}

coordinate_t move(coordinate_t const& current_point, Heading const& heading, int distance) {
    auto tmp = current_point;

    switch (heading) {
        case Heading::North: tmp.first += distance; break;
        case Heading::South: tmp.first -= distance; break;
        case Heading::East: tmp.second += distance; break;
        case Heading::West: tmp.second -= distance; break;
        default:
            throw std::invalid_argument("invalid heading");
    }

    return tmp;
}

std::vector<coordinate_t> calculate_path_points(coordinate_t const& current_point, Heading const& heading, int distance) {
    auto tmp = current_point;
    auto rv = std::vector<coordinate_t>();
    for (auto i = 0; i < distance; ++i) {
        tmp = move(tmp, heading, 1);
        rv.push_back(tmp);
    }
    return rv;
}

std::pair<int, int> process_commands(std::vector<command_t> const& commands) {
    auto current_point = std::make_pair(0, 0);
    bool already_visited = false;
    auto already_visited_point = current_point;
    auto heading = Heading::North;
    auto locations = std::set<coordinate_t>{current_point};
    for (auto const& command : commands) {
        heading = rotate(heading, command.first);

        if (!already_visited) {
            auto points = calculate_path_points(current_point, heading, command.second);
            for (auto const& point : points) {
                if (locations.count(point)) {
                    already_visited = true;
                    already_visited_point = point;
                    locations.clear();
                }
                else {
                    locations.insert(point);
                }
            }
        }
        current_point = move(current_point, heading, command.second);
    }

    auto already_visited_distance = calculate_distance(already_visited_point);
    auto final_distance = calculate_distance(current_point);
    return { final_distance, already_visited_distance };
}

