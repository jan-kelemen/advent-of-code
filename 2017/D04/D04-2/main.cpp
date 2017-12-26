#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using passphrase_t = std::vector<std::string>;

std::vector<passphrase_t> read_passphrases(std::istream& stream);

bool is_valid_passhprase(passphrase_t const& passphrase);

int count_valid_passphrases(std::vector<passphrase_t> const& passphrases);

int main(int argc, char** argv)
{
    auto passphrases = read_passphrases(std::cin);
    auto valid = count_valid_passphrases(passphrases);
    std::cout << valid << '\n';
}

std::vector<std::vector<std::string>> read_passphrases(std::istream& stream)
{
    auto rv = std::vector<std::vector<std::string>>();

    std::string line;
    while(std::getline(stream, line))
    {
        auto passphrase = passphrase_t();

        auto word_stream = std::istringstream(line);

        std::string word;
        while(word_stream >> word)
        {
            passphrase.push_back(word);
        }

        rv.emplace_back(std::move(passphrase));
    }

    return rv;
}

bool is_valid_passhprase(passphrase_t const& passphrase)
{
    auto set = std::unordered_set<std::string>();
    for(auto const& word : passphrase)
    {
        for(auto const& seen_word : set)
        {
            if(std::is_permutation(seen_word.begin(), seen_word.end(), word.begin(), word.end()))
            {
                return false;
            }
        }
        set.insert(word);
    }
    return true;
}

int count_valid_passphrases(std::vector<passphrase_t> const& passphrases)
{
    return std::count_if(passphrases.begin(), passphrases.end(), [](passphrase_t const& p) {
        return is_valid_passhprase(p);
    });
}
