#include <iostream>
#include <string>

std::string read_stream(std::istream& stream);

int garbage_items(std::string const& stream);

int main(int argc, char** argv)
{
    auto garbaged_stream = read_stream(std::cin);
    auto garbage = garbage_items(garbaged_stream);
    std::cout << garbage << '\n';
}

std::string read_stream(std::istream& stream)
{
    std::string rv;
    std::getline(stream, rv);
    return rv;
}

int garbage_items(std::string const& stream)
{
    auto rv = 0;
    auto in_garbage = false;
    for(size_t i = 0; i < stream.size(); ++i)
    {
        if(in_garbage)
        {
            if(stream[i] == '>')
            {
                in_garbage = false;
            }
            else if(stream[i] == '!')
            {
                ++i;
            }
            else
            {
                ++rv;
            }
        }
        else if (stream[i] == '<')
        {
            in_garbage = true;
        }
    }
    return rv;
}
