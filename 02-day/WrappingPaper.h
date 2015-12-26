#ifndef WRAPPING_PAPER_H

#define WRAPPING_PAPER_H

#include <cstdint>

#include "Present.h"

using area_t = uint32_t;

class WrappingPaper
{
public:
    static WrappingPaper for_present(const Present &present);

    WrappingPaper(const area_t area);
    
    area_t area() const { return area_; }

    ~WrappingPaper() = default;

private:
    area_t area_;

};

#endif //WRAPPING_PAPER_H