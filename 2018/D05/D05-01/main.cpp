#include <cctype>
#include <iostream>
#include <string>
#include <iterator>

int main()
{
    using namespace std::string_literals;

    auto polymers = ""s;
    auto last_char = '\0';
    for(auto it = std::istream_iterator<char>{std::cin}; it != std::istream_iterator<char>(); ++it)
    {
        char const current_char = *it;
        if (!std::isalpha(current_char))
            continue;

        if (std::tolower(last_char) != std::tolower(current_char) || last_char == current_char)
        {
            last_char = current_char;
            polymers.push_back(last_char);
            continue;
        }

        polymers.pop_back();
        last_char = polymers.empty() ? '\0' : *polymers.rbegin();
    }

    std::cout << polymers.size() << '\n';
}
