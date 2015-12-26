#include <chrono>
#include <future>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

#include "Command.h"
#include "ControllableLightGrid.h"
#include "LightGrid.h"
#include "Worker.h"

std::vector<std::string> get_instructions(std::istream &input)
{
    auto instructions = std::vector<std::string>();

    if (input.good())
    {
        auto instruction = std::string();
        while (std::getline(input, instruction))
        {
            instructions.push_back(instruction);
        }
    }

    return instructions;
}

size_t follow_commands(const std::vector<Command> commands, LightGrid &grid)
{
    auto worker = Worker();
    worker.execute(commands, grid);
    return grid.turned_on();
}

int main()
{    
    auto commands = Command::from_instructions(get_instructions(std::ifstream("input.txt")));

    auto old_lights = std::async(follow_commands, commands, LightGrid());
    auto new_lights = std::async(follow_commands, commands, ControllableLightGrid());

    std::cout << "We will need " << old_lights.get() << " new light bulbs after this year.\n";

    std::cout << "Ok, ok, we will buy the new lights now.\n";
    std::cout << "New lights shine with " << new_lights.get() << " brightness.\n";
    return 0;
}