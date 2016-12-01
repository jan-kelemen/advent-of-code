#include "Worker.h"

void Worker::execute(const std::vector<Command> &commands, LightGrid &grid)
{
    for (const auto &command : commands)
    {
        execute(command, grid);
    }
}

void Worker::execute(const Command &command, LightGrid &grid)
{
    switch (command.action())
    {
    case Action::TURN_ON:
        grid.turn_on(command.coordinate());
        break;
    case Action::TURN_OFF:
        grid.turn_off(command.coordinate());
        break;
    case Action::TOGGLE:
        grid.toggle(command.coordinate());
        break;
    }
}
