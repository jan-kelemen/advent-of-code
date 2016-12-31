#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Direction.h"

using instruction_t = std::vector<Direction>;
using keypad_t = std::vector<std::vector<char>>;

std::vector<instruction_t> read_instructions(char const* filename);

std::vector<char> process_keypad_instructions(std::vector<instruction_t> const& commands, size_t vertical, size_t horizontal, keypad_t const& keypad);

int main() {
    auto constexpr input_filename = "input.txt";
    auto commands = read_instructions(input_filename);
    auto part1_result = process_keypad_instructions(commands, 1, 1, {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'}
    });

    auto part2_result = process_keypad_instructions(commands, 2, 0, {
        {0, 0, '1', 0, 0},
        {0, '2', '3', '4', 0},
        {'5', '6', '7', '8', '9'},
        {0, 'A', 'B', 'C', 0},
        {0, 0, 'D', 0, 0}
    });

    for (auto digit : part1_result) {
        std::cout << digit;
    }
    std::cout << ", ";
    for (auto digit : part2_result) {
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

std::vector<char> process_keypad_instructions(std::vector<instruction_t> const& commands, size_t vertical, size_t horizontal, keypad_t const& keypad) {
    auto rv = std::vector<char>();

    for (auto const& command : commands) {
        auto ver = vertical;
        auto hor = horizontal;

        for (auto& direction : command) {
            switch (direction) {
                case Direction::Up:
                    if (ver != 0 && keypad[ver - 1][hor] != 0) { --ver; } break;
                case Direction::Down:
                    if (ver + 1 != keypad.size() && keypad[ver + 1][hor] != 0) { ++ver; } break;
                case Direction::Left:
                    if (hor != 0 && keypad[ver][hor - 1] != 0) { --hor; } break;
                case Direction::Right:
                    if (hor + 1 != keypad[ver].size() && keypad[ver][hor + 1] != 0) { ++hor; } break;
                default:
                    break;
            }
        }
        rv.push_back(keypad[ver][hor]);
    }

    return rv;
}
