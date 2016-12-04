#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Direction.h"

using instruction_t = std::vector<Direction>;

std::vector<instruction_t> read_instructions(char const* filename);

std::vector<int> process_instructions(std::vector<instruction_t> const& commands);

int main() {
    auto constexpr input_filename = "input.txt";
    auto commands = read_instructions(input_filename);
    auto result = process_instructions(commands);

    for (auto digit : result) {
        std::cout << digit;
    }
    std::cout << '\n';
    system("pause");
}

std::vector<instruction_t> read_instructions(char const* filename) {
    auto rv = std::vector<instruction_t>();
    std::ifstream input_file(filename);

    std::string line;
    while (std::getline(input_file, line)) {
        auto instruction = instruction_t();
        std::for_each(line.begin(), line.end(), [&instruction](auto c) {
            if (c == 'U') { instruction.push_back(Direction::Up); }
            if (c == 'D') { instruction.push_back(Direction::Down); }
            if (c == 'R') { instruction.push_back(Direction::Right); }
            if (c == 'L') { instruction.push_back(Direction::Left); }
        });
        rv.push_back(instruction);
    }

    return rv;
}

std::vector<int> process_instructions(std::vector<instruction_t> const& commands) {
    auto rv = std::vector<int>();

    for (auto const& command : commands) {
        auto number = 5;
        for (auto& direction : command) {
            switch (direction) {
                case Direction::Up:
                    if (number - 3 > 0) { number -= 3; } break;
                case Direction::Down:
                    if (number + 3 < 10) { number += 3; } break;
                case Direction::Left:
                    if (number % 3 != 1) { --number; } break;
                case Direction::Right:
                    if (number % 3 != 0) { ++number; } break;
                default:
                    break;
            }
        }
        rv.push_back(number);
    }
    
    return rv;
}
