#include <algorithm>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

using triangle_sides_t = std::tuple<int, int, int>;

std::vector<triangle_sides_t> read_triangles(char const* filename);

int main() {
    auto constexpr input_filename = "input.txt";
    auto triangles = read_triangles(input_filename);
    auto valid_triangles_count = std::count_if(triangles.begin(), triangles.end(), [](auto& t) {
        return (std::get<0>(t) + std::get<1>(t) > std::get<2>(t)) &&
            (std::get<1>(t) + std::get<2>(t) > std::get<0>(t)) &&
            (std::get<0>(t) + std::get<2>(t) > std::get<1>(t));
    });

    std::cout << valid_triangles_count << '\n';
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
