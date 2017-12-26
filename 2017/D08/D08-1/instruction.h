#pragma once

#include <map>

#include "condition.h"

class instruction
{
public:
    enum class type
    {
        inc, dec
    };

    instruction(type type, int value, condition condition, std::string reg);

    void instruction::calculate(std::map<std::string, int>& registers) const;

private:
    type type_;
    int value_;
    condition condition_;
    std::string reg_;
};