#ifndef COMMAND_H

#define COMMAND_H

#include <string>
#include <vector>

#include "LightGrid.h"

enum Action { TURN_ON, TURN_OFF, TOGGLE };

class Command
{
public:

    static std::vector<Command> from_instructions(const std::vector<std::string> &instructions);

    Command(const Action &action, const coordinate_t &coordinate);

    Action action() const { return action_; }

    coordinate_t coordinate() const { return coordinate_; }

private:
    Action action_;
    coordinate_t coordinate_;
};

#endif COMMAND_H