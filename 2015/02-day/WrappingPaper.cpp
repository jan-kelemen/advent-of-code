#include <algorithm>

#include "WrappingPaper.h"

WrappingPaper WrappingPaper::for_present(const Present & present)
{
    auto length = present.length();
    auto width = present.width();
    auto height = present.height();

    auto side1 = area_t(length * width);
    auto side2 = area_t(width * height);
    auto side3 = area_t(height * length);

    return WrappingPaper(2 * (side1 + side2 + side3) + std::min({ side1, side2, side3 }));
}


WrappingPaper::WrappingPaper(const area_t area) : area_(area) {}

