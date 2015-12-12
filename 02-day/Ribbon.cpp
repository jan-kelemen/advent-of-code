#include <algorithm>

#include "Ribbon.h"

Ribbon Ribbon::for_present(const Present &present)
{
    auto length = present.length();
    auto width = present.width();
    auto height = present.height();

    auto side1 = 2 * (length + width);
    auto side2 = 2 * (width + height);
    auto side3 = 2 * (height + length);

    auto volume = length * width * height;

    return Ribbon(std::min({ side1, side2, side3 }) + dimension_t(volume));
}

Ribbon::Ribbon(const dimension_t &length) : length_(length) {}