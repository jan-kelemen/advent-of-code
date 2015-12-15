#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <functional>
#include <regex>
#include <set>
#include <string>
#include <thread>

std::vector<std::string> get_strings(std::istream &input)
{
    auto strings = std::vector<std::string>();

    if (input.good())
    {
        auto line = std::string();
        while (input >> line)
        {
            strings.push_back(line);
        }
    }

    return strings;
}

std::function<bool(const std::string &)> old_rules()
{
    auto vowels = std::set<char>({ 'a', 'e', 'i', 'o', 'u' });
    auto is_vowel = [vowels](const char symbol) { return vowels.count(symbol) != 0; };
    auto has_vowels = [is_vowel](const std::string &string, const size_t n = 3) { return std::count_if(std::begin(string), std::end(string), is_vowel) >= n; };

    auto illegal_pair = std::regex(R"(.*(ab|cd|pq|xy).*)");
    auto has_illegal_pair = [illegal_pair](const std::string &string) { return std::regex_match(string, illegal_pair); };

    auto double_symbol = std::regex(R"(.*(.)\1.*)");
    auto has_double_symbols = [double_symbol](const std::string &string) { return std::regex_match(string, double_symbol); };

    auto is_nice = [has_illegal_pair, has_double_symbols, has_vowels](const std::string &string) {
        return !has_illegal_pair(string)
            && has_double_symbols(string)
            && has_vowels(string);
    };

    return is_nice;
}

std::function<bool(const std::string &)> new_rules()
{
    auto double_pair = std::regex(R"(.*(..).*\1.*)");
    auto has_double_pair = [double_pair](const std::string &string) { return std::regex_match(string, double_pair); };

    auto separated_pair = std::regex(R"(.*(.)(.)\1.*)");
    auto has_separated_pair = [separated_pair](const std::string &string) { return std::regex_match(string, separated_pair); };

    auto is_nice = [has_double_pair, has_separated_pair](const std::string &string) { return has_double_pair(string) && has_separated_pair(string); };
    
    return is_nice;
}

std::vector<std::string> filter_nice(const std::vector<std::string> &strings, const std::function<bool(std::string)> &is_nice)
{
    auto nice_strings = std::vector<std::string>(strings.size());

    auto it = std::copy_if(std::begin(strings), std::end(strings), std::begin(nice_strings), is_nice);

    nice_strings.resize(std::distance(std::begin(nice_strings), it));

    return nice_strings;
}

int main()
{
    auto strings = get_strings(std::ifstream("input.txt"));

    auto old_nice_strings = filter_nice(strings, old_rules());
    std::cout << "There was " <<  old_nice_strings.size() << " nice strings this year.\n";

    std::cout << "Wait! ";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto new_nice_strings = filter_nice(strings, new_rules());
    std::cout << "We have " << new_nice_strings.size() << " really nice ones.\n";

    return 0;
}