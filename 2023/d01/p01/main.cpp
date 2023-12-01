#include <fstream>
#include <iostream>
#include <string>

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input{argv[1]};

    int calibration_sum{};
    std::string line;
    while (getline(input, line))
    {
        int const first_digit = line[line.find_first_of("0123456789")] - '0';
        int const second_digit = line[line.find_last_of("0123456789")] - '0';

        calibration_sum += first_digit * 10 + second_digit;
    }
    std::cout << calibration_sum << '\n';
}
