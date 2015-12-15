#include <algorithm>
#include <regex>
#include <tuple>

#include "Command.h"

Action parse_action(const std::string &action)
{
    if (action == "turn on")
    {
        return Action::TURN_ON;
    }
    else if (action == "turn off")
    {
        return Action::TURN_OFF;
    }
    else
    {
        return Action::TOGGLE;
    }
}

std::vector<Command> Command::from_instructions(const std::vector<std::string> &instructions)
{
    auto commands = std::vector<Command>();

    auto pattern = std::regex(R"((turn on|turn off|toggle) (\d*),(\d*) through (\d*),(\d*))");

    for (const auto &instruction : instructions)
    {
        auto match_results = std::smatch();
        std::regex_match(instruction, match_results, pattern);

        auto action = parse_action(match_results[1]);
        auto from = coordinate_t(std::stoi(match_results[2]), std::stoi(match_results[3]));
        auto to = coordinate_t(std::stoi(match_results[4]), std::stoi(match_results[5]));

        size_t x_init;
        size_t x_fin;
        std::tie(x_init, x_fin) = std::minmax(from.first, to.first);

        size_t y_init;
        size_t y_fin;
        std::tie(y_init, y_fin) = std::minmax(from.second, to.second);

        for (auto x = x_init; x <= x_fin; ++x)
        {
            for (auto y = y_init; y <= y_fin; ++y)
            {
                commands.push_back({ action, { x, y } });
            }
        }
    }

    return commands;
}



Command::Command(const Action &action, const coordinate_t &coordinate) : action_(action), coordinate_(coordinate) {}