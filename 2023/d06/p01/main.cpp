#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

std::vector<int> extract_digits(std::string const& line)
{
    std::vector<int> rv;

    std::regex re{R"(\d+)"};
    std::sregex_iterator it{line.cbegin(), line.cend(), re}, it_end;
    for (; it != it_end; ++it)
    {
        rv.push_back(std::stoi(it->str()));
    }

    return rv;
}

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input{argv[1]};

    std::string line;

    std::getline(input, line);
    auto times{extract_digits(line)};

    std::getline(input, line);
    auto distances{extract_digits(line)};

    size_t result{1};
    for (size_t i{0}; i != times.size(); ++i)
    {
        auto time{times[i]};
        auto distance{distances[i]};
        size_t count{};
        for (size_t j{1}; j != time - 1; ++j)
        {
            if (distance < time * j - j * j)
            {
                ++count;
            }
        }
        result *= count;
    }
    std::cout << result << '\n';
}
