#ifndef PRESENT_H

#define PRESENT_H

#include <string>

using dimension_t = uint32_t;
using area_t = uint32_t;

class Present
{
public:
    Present static from_dimensions(const std::string &dimensions);

    Present(const dimension_t &length, const dimension_t &width, const dimension_t &height);

    dimension_t length() const { return length_; }
    dimension_t width() const { return width_; }
    dimension_t height() const { return height_; }

    ~Present() = default;

private:
    dimension_t length_;
    dimension_t width_;
    dimension_t height_;

};

#endif //PRESENT_H
