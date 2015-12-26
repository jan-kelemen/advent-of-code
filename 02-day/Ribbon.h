#ifndef RIBBON_H

#define RIBBON_H

#include "Present.h"

class Ribbon
{
public:
    static Ribbon for_present(const Present &present);
    
    Ribbon(const dimension_t &length);

    dimension_t length() const { return length_; }

    ~Ribbon() = default;

private:
    dimension_t length_;
};

#endif //RIBBON_H