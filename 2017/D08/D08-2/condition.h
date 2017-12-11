#pragma once

#include <string>

class condition
{
public:
    enum class type
    {
        l, le, g, ge, e, ne
    };

    condition(type type, int value, std::string reg);

    bool evaluate(int register_value) const;

    std::string const& reg() const;
private:
    type type_;
    int value_;
    std::string reg_;
};