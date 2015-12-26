#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Present.h"
#include "Ribbon.h"
#include "WrappingPaper.h"

std::vector<Present> get_presents(std::istream &input)
{
    auto presents = std::vector<Present>();
    
    if (input.good())
    {
        auto dimensions = std::string();
        while (input >> dimensions)
        {
            presents.push_back(Present::from_dimensions(dimensions));
        }
    }

    return presents;
}

std::vector<WrappingPaper> get_wrapping_papers(const std::vector<Present> &presents)
{
    auto wrapping_papers = std::vector<WrappingPaper>();

    for (const auto &present : presents)
    {
        wrapping_papers.push_back(WrappingPaper::for_present(present));
    }

    return wrapping_papers;
}

area_t calculate_wrapping_paper_area(const std::vector<WrappingPaper> &papers)
{
    auto area = area_t(0);
    
    for (const auto &paper : papers)
    {
        area += paper.area();
    }

    return area;
}

std::vector<Ribbon> get_ribbons(const std::vector<Present> &presents)
{
    auto ribbons = std::vector<Ribbon>();

    for (const auto &present : presents)
    {
        ribbons.push_back(Ribbon::for_present(present));
    }

    return ribbons;
}

dimension_t calculate_ribbon_length(const std::vector<Ribbon> &ribbons)
{
    auto length = dimension_t(0);

    for (const auto &ribbon : ribbons)
    {
        length += ribbon.length();
    }

    return length;
}
int main()
{
    auto presents = get_presents(std::ifstream("input.txt"));

    auto wrapping_papers = get_wrapping_papers(presents);
    auto area = calculate_wrapping_paper_area(wrapping_papers);
    std::cout << "Elves will need " << area << " square feet of wrapping paper.\n";

    auto ribbons = get_ribbons(presents);
    auto length = calculate_ribbon_length(ribbons);
    std::cout << "Elves will need " << length << " feet of ribbon.\n";

    return EXIT_SUCCESS;
}