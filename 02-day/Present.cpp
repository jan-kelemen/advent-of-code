#include "Present.h"

#include <regex>

Present Present::from_dimensions(const std::string &dimensions)
{
    auto dimension_matcher = std::regex(R"((\d+)x(\d+)x(\d+))");
    auto match_results = std::smatch();
    std::regex_match(dimensions, match_results, dimension_matcher);

    auto length = dimension_t(std::stoi(match_results[1]));
    auto width = dimension_t(std::stoi(match_results[2]));
    auto height = dimension_t(std::stoi(match_results[3]));

    return Present(length, width, height);
}

Present::Present(const dimension_t &length, const dimension_t &width, const dimension_t &height) : length_(length), width_(width), height_(height) {}