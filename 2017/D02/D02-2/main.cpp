#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using spreadsheet_t = std::vector<std::vector<int>>;

spreadsheet_t read_spreadsheet(std::istream& stream);

int spreadsheet_checksum(spreadsheet_t const& spreadsheet);

int main(int argc, char **argv)
{
    auto spreadsheet = read_spreadsheet(std::cin);
    auto checksum = spreadsheet_checksum(spreadsheet);
    std::cout << checksum << '\n';
}

spreadsheet_t read_spreadsheet(std::istream& stream)
{
    auto rv = spreadsheet_t();

    std::string line;
    while(std::getline(stream, line))
    {
        auto row = std::vector<int>();
        auto string_stream = std::istringstream(line);
        auto value = 0;
        while(string_stream >> value) { row.push_back(value); }
        rv.emplace_back(std::move(row));
    }

    return rv;
}

int spreadsheet_checksum(spreadsheet_t const& spreadsheet)
{
    return std::accumulate(
        spreadsheet.begin(), spreadsheet.end(), 0,
        [](int current_sum, auto const& row) {
            for(auto it = row.begin(); it != row.end() - 1; ++it)
            {
                for(auto other = it + 1; other != row.end(); ++other)
                {
                    auto[min, max] = std::minmax(*it, *other);
                    if(max % min == 0)
                    {
                        return current_sum + max / min;
                    }
                }
            }
            return current_sum;
        }
    );
}
