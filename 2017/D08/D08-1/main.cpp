#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "instruction.h"

using registers_t = std::map<std::string, int>;

std::vector<instruction> read_instructions(std::istream& stream);

void execute_instructions(std::vector<instruction> const& instructions, registers_t& registers);

int largest_value_in_registers(registers_t const& registers);

int main(int argc, char** argv)
{
    auto instructions = read_instructions(std::cin);
    auto registers = registers_t();
    execute_instructions(instructions, registers);
    auto largest = largest_value_in_registers(registers);
    std::cout << largest << '\n';
}

std::vector<instruction> read_instructions(std::istream& stream)
{
    std::vector<instruction> rv;

    std::string line;
    while(std::getline(stream, line))
    {
        std::string destination;
        std::string instruction;
        int instruction_value;
        std::string ignore;
        std::string condition_register;
        std::string condition;
        int condition_value;

        auto sstream = std::istringstream(line);

        sstream >> destination >> instruction >> instruction_value
            >> ignore >> condition_register >> condition >> condition_value;

        auto instruction_type = instruction == "inc" ? instruction::type::inc : instruction::type::dec;
        auto condition_type = [&condition]() {
            if(condition == "<") { return condition::type::l; }
            if(condition == "<=") { return condition::type::le; }
            if(condition == ">") { return condition::type::g; }
            if(condition == ">=") { return condition::type::ge; }
            if(condition == "==") { return condition::type::e; }
            if(condition == "!=") { return condition::type::ne; }

            throw 1;
        }();

        auto condition_ = ::condition(condition_type, condition_value, condition_register);
        auto instruction_ = ::instruction(instruction_type, instruction_value, condition_, destination);

        rv.emplace_back(std::move(instruction_));
    }
    return rv;
}

void execute_instructions(std::vector<instruction> const& instructions, registers_t& registers)
{
    std::for_each(instructions.begin(), instructions.end(), [&registers](auto const& instruction) {
        instruction.calculate(registers);
    });
}

int largest_value_in_registers(registers_t const& registers)
{
    auto const& max_element = std::max_element(registers.begin(), registers.end(), [](auto const& first, auto const& second) {
        return first.second < second.second;
    });

    return max_element->second;
}
