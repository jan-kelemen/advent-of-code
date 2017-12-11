#include "instruction.h"

instruction::instruction(type type, int value, condition condition, std::string reg)
    : type_{ type }, value_{ value }, condition_{ condition }, reg_{ std::move(reg) }
{
    ;
}

void instruction::calculate(std::map<std::string, int>& registers) const
{
    if(condition_.evaluate(registers[condition_.reg()]))
    {
        switch(type_)
        {
        case type::inc:
            registers[reg_] += value_;
            break;
        case type::dec:
            registers[reg_] -= value_;
            break;
        }
    }
}
