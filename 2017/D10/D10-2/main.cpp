#include <algorithm>
#include <functional>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

std::vector<int> read_lenghts(std::istream& stream);

std::vector<uint8_t> encode(std::vector<int> const& lenghts);

std::string convert_to_hex_string(std::vector<uint8_t> hash);

auto constexpr list_size = 256;

int main(int argc, char** argv)
{
    auto lenghts = read_lenghts(std::cin);
    auto list = encode(lenghts);
    auto str = convert_to_hex_string(list);
    std::cout << str << '\n';
}

std::vector<int> read_lenghts(std::istream& stream)
{
    auto rv = std::vector<int>();
    char c;
    while(stream >> c)
    {
        rv.push_back(c);
    }

    rv.push_back(17);
    rv.push_back(31);
    rv.push_back(73);
    rv.push_back(47);
    rv.push_back(23);

    return rv;
}

std::vector<int> generate_initial_list(size_t size)
{
    auto rv = std::vector<int>(size);
    std::generate(rv.begin(), rv.end(), [n = 0]() mutable { return n++; });
    return rv;
}

void reverse(std::vector<int>& list, int begin, int end)
{
    auto elements_to_swap = (end - begin + 1) / 2;
    auto i = begin % list_size;
    auto j = end % list_size;
    while(elements_to_swap != 0)
    {
        std::swap(list[i++], list[j--]);
        if(i == list_size) { i = 0; }
        if(j < 0) { j = list_size - 1; }
        --elements_to_swap;
    }
}

std::vector<uint8_t> encode(std::vector<int> const& lenghts)
{
    auto sparse = generate_initial_list(list_size);

    auto current_position = 0;
    auto skip_size = 0;

    for(auto i = 0; i < 64; ++i)
    {
        for(auto const& lenght : lenghts)
        {
            reverse(sparse, current_position, current_position + lenght - 1);
            current_position += lenght + skip_size++;
            current_position %= list_size;
        }
    }
    
    auto dense = std::vector<uint8_t>();

    for(auto i = 0; i < 16; ++i)
    {
        auto offset = i * 16;
        auto hash = std::accumulate(sparse.begin() + offset, sparse.begin() + offset + 16, uint8_t(0), std::bit_xor<uint8_t>());
        dense.push_back(hash);
    }

    return dense;
}

std::string convert_to_hex_string(std::vector<uint8_t> hash)
{
    std::ostringstream stream;
    stream << std::hex;
    std::for_each(hash.begin(), hash.end(), [&stream](int i) { stream << std::setw(2) << std::setfill('0') << i; });
    return stream.str();
}
