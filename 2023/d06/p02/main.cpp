#include <charconv>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

uintmax_t extract_digits(std::string const& line)
{
    std::string tmp;

    std::regex re{R"(\d+)"};
    std::sregex_iterator it{line.cbegin(), line.cend(), re}, it_end;
    for (; it != it_end; ++it)
    {
        tmp += it->str();
    }

    uintmax_t rv;
    std::from_chars(tmp.data(), tmp.data() + tmp.size(), rv);
    return rv;
}

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input{argv[1]};

    std::string line;

    std::getline(input, line);
    auto time{extract_digits(line)};

    std::getline(input, line);
    auto distance{extract_digits(line)};

    size_t count{};
    for (uintmax_t j{1}; j != time - 1; ++j)
    {
        if (distance < time * j - j * j)
        {
            ++count;
        }
    }
    std::cout << count << '\n';
}
