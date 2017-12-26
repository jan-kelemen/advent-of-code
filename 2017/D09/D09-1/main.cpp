#include <iostream>
#include <string>

std::string read_stream(std::istream& stream);

std::string garbage_items(std::string const& stream);

int calculate_score(std::string const& stream);

int main(int argc, char** argv)
{
    auto garbaged_stream = read_stream(std::cin);
    auto clean_stream = garbage_items(garbaged_stream);
    auto score = calculate_score(clean_stream);
    std::cout << score << '\n';
}

std::string read_stream(std::istream& stream)
{
    std::string rv;
    std::getline(stream, rv);
    return rv;
}

std::string garbage_items(std::string const& stream)
{
    std::string rv;
    auto in_garbage = false;
    for(size_t i = 0; i < stream.size(); ++i)
    {
        if(stream[i] == '!')
        {
            ++i; continue;
        }
        else if(stream[i] == ',')
        {
            continue;
        }
        else if(stream[i] == '<')
        {
            in_garbage = true;
            continue;
        }
        else if(in_garbage && stream[i] == '>')
        {
            in_garbage = false;
            continue;
        }

        if(!in_garbage)
        {
            rv.push_back(stream[i]);
        }
    }
    return rv;
}

int calculate_score(std::string const& stream) 
{
    auto rv = 0;
    auto current = 0;
    for(auto const& c : stream)
    {
        if(c == '{') { ++current; rv += current; }
        else { --current; }
    }
    return rv;
}
