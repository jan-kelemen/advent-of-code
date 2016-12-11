#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::vector<std::string> read_rooms(char const* filename);

bool check_room_validity(std::string const& room_name);

int print_decoded_and_sum_sectors(std::vector<std::string> const& rooms);

int main() {
    auto constexpr input_filename = "input.txt";
    auto rooms = read_rooms(input_filename);

    std::cout << print_decoded_and_sum_sectors(rooms) << '\n';

    system("pause");
}

std::vector<std::string> read_rooms(char const* filename) {
    auto rv = std::vector<std::string>();

    std::ifstream input_file(filename);

    std::string line;
    while (std::getline(input_file, line)) { rv.push_back(line); }

    return rv;
}

bool check_room_validity(std::string const& room_name) {
    auto frequencies = std::map<char, int>();
    for (auto it = room_name.begin(); !isdigit(*it); ++it) {
        if (*it == '-') { continue; }
        ++frequencies[*it];
    }

    auto tmp = std::vector<std::pair<char, int>>();
    for (auto& p : frequencies) {
        tmp.emplace_back(p.first, p.second);
    }
    std::sort(tmp.begin(), tmp.end(), [](auto const & a, auto const & b) {
        return a.second != b.second ? a.second > b.second : a.first < b.first;
    });
    std::string checksum;
    std::for_each(tmp.begin(), tmp.begin() + 5, [&checksum](auto &p) {checksum.push_back(p.first); });

    auto rv = room_name.find(checksum, room_name.find('[')) != std::string::npos;

    return rv;
}

void rotate_and_print(std::string const& s, int d) {
    for (auto c : s) {
        std::cout << char('a' + ((int(c - 'a') + d) % 26)); 
    }
    std::cout << '\n';
}

int print_decoded_and_sum_sectors(std::vector<std::string> const& rooms) {
    auto rv = 0;
    for (auto const& room : rooms) {
        if (!check_room_validity(room)) { continue; }

        auto first = room.find_first_of("0123456789");
        auto second = room.find_first_of("[");
        auto sector = std::stoi(room.substr(first, second - first));

        rotate_and_print(room, sector);

        rv += sector;
    }
    return rv;
}

