#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

std::vector<int> read_lenghts(std::istream& stream);

std::vector<int> encode(std::vector<int> const& lenghts);

int calculate_result(std::vector<int> const& sequence);

auto constexpr list_size = 256;

int main(int argc, char** argv)
{
    auto lenghts = read_lenghts(std::cin);
    auto list = encode(lenghts);
    auto result = calculate_result(list);
    std::cout << result << '\n';
}

std::vector<int> read_lenghts(std::istream& stream)
{
    auto rv = std::vector<int>();
    std::string str;
    while(std::getline(stream, str, ','))
    {
        rv.push_back(std::stoi(str));
    }
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

std::vector<int> encode(std::vector<int> const& lenghts)
{
    auto list = generate_initial_list(list_size);
    
    auto current_position = 0;
    auto skip_size = 0;

    for(auto const& lenght : lenghts)
    {
        reverse(list, current_position, current_position + lenght - 1);
        current_position += lenght + skip_size++;
        current_position %= list_size;
    }

    return list;
}

int calculate_result(std::vector<int> const& sequence)
{
    return sequence[0] * sequence[1];
}

