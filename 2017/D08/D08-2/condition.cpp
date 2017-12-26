#include "condition.h"

condition::condition(type type, int value, std::string reg)
    : type_{ type }, value_{ value }, reg_{ std::move(reg) }
{
    ;
}

bool condition::evaluate(int register_value) const
{
    switch(type_)
    {
    case type::l:
        return register_value < value_;
    case type::le:
        return register_value <= value_;
    case type::g:
        return register_value > value_;
    case type::ge:
        return register_value >= value_;
    case type::e:
        return register_value == value_;
    }

    return register_value != value_;
}

std::string const& condition::reg() const
{
    return reg_;
}
