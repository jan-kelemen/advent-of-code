#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <stack>
#include <string>

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input {argv[1]};

    std::map<int, std::string> raw_stack_state;
    std::regex const stack_extract {
        R"(.([[:alpha:]]| )...([[:alpha:]]| )...([[:alpha:]]| )...([[:alpha:]]| )...([[:alpha:]]| )...([[:alpha:]]| )...([[:alpha:]]| )...([[:alpha:]]| )...([[:alpha:]]| ).)"};
    std::string line;
    while (std::getline(input, line) && !line.empty())
    {
        std::smatch m;
        if (std::regex_match(line, m, stack_extract))
        {
            for (auto i {1}; i != 10; ++i)
            {
                auto value {m[i].str()};
                if (value != " ")
                {
                    raw_stack_state[i] = value + raw_stack_state[i];
                }
            }
        }
    }

    std::map<int, std::stack<char>> stack_state;
    std::transform(std::cbegin(raw_stack_state),
        std::cend(raw_stack_state),
        std::inserter(stack_state, std::begin(stack_state)),
        [](auto&& v)
        {
            std::stack<char> s;
            for (auto c : v.second)
            {
                s.push(c);
            }
            return std::pair<int, std::stack<char>> {v.first, s};
        });

    // move 1 from 2 to 1
    std::regex move_extract {R"(move (\d+) from (\d) to (\d))"};
    while (std::getline(input, line))
    {
        std::smatch m;
        if (std::regex_match(line, m, move_extract))
        {
            auto const count = std::stoi(m[1].str());
            auto& from = stack_state[std::stoi(m[2].str())];
            auto& to = stack_state[std::stoi(m[3].str())];

            for (auto i {0}; i != count; ++i)
            {
                auto crate {from.top()};
                from.pop();
                to.push(crate);
            }
        }
    }

    for (auto&& s : stack_state)
    {
        std::cout << s.second.top();
    }
    std::cout << '\n';
}
