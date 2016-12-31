#include <algorithm>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

using triangle_sides_t = std::tuple<int, int, int>;

std::vector<triangle_sides_t> read_triangles(char const* filename);

int count_valid_triangles(std::vector<triangle_sides_t> const& triangles);

void transform_input(std::vector<triangle_sides_t>& triangles);

int main() {
    auto constexpr input_filename = "input.txt";
    auto triangles = read_triangles(input_filename);
    auto horizontal_triangles = count_valid_triangles(triangles);
    transform_input(triangles);
    auto vertical_triangles = count_valid_triangles(triangles);

    std::cout << horizontal_triangles << ", " << vertical_triangles << '\n';
    system("pause");
}

std::vector<triangle_sides_t> read_triangles(char const* filename) {
    auto rv = std::vector<triangle_sides_t>();

    std::ifstream input_file(filename);

    int first, second, third;
    while (input_file >> first >> second >> third) {
        rv.emplace_back(first, second, third);
    }

    return rv;
}

int count_valid_triangles(std::vector<triangle_sides_t> const& triangles) {
    return std::count_if(triangles.begin(), triangles.end(), [](auto& t) {
        return (std::get<0>(t) + std::get<1>(t) > std::get<2>(t)) &&
            (std::get<1>(t) + std::get<2>(t) > std::get<0>(t)) &&
            (std::get<0>(t) + std::get<2>(t) > std::get<1>(t));
    });
}

void transform_input(std::vector<triangle_sides_t>& triangles) {
    for (size_t i = 0; i < triangles.size() - 2; i += 3) {
        std::swap(std::get<0>(triangles[i + 1]), std::get<1>(triangles[i]));
        std::swap(std::get<0>(triangles[i + 2]), std::get<2>(triangles[i]));
        std::swap(std::get<1>(triangles[i + 2]), std::get<2>(triangles[i + 1]));
    }
}
