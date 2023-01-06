#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

enum class pick
{
    rock = 1,
    paper = 2,
    scissors = 3
};

enum class outcome
{
    lose,
    draw,
    win
};

struct round
{
    pick player_one;
    pick player_two;
};

std::istream& operator>>(std::istream& s, round& r)
{
    r.player_one = static_cast<pick>(s.get() - 'A' + 1);
    s.ignore();
    r.player_two = [](pick const other, outcome const o)
    {
        switch (o)
        {
        case outcome::lose:
            if (other == pick::rock)
            {
                return pick::scissors;
            }
            return static_cast<pick>(static_cast<int>(other) - 1);
        case outcome::win:
            if (other == pick::scissors)
            {
                return pick::rock;
            }
            return static_cast<pick>(static_cast<int>(other) + 1);
        default:
            return other;
        }
    }(r.player_one, static_cast<outcome>(s.get() - 'X'));
    s.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return s;
}

uint64_t as_score(round const& r)
{
    uint64_t const outcome = [](round const& r)
    {
        if (r.player_one == r.player_two)
        {
            return 3;
        }
        if (r.player_one == pick::rock && r.player_two == pick::paper ||
            r.player_one == pick::paper && r.player_two == pick::scissors ||
            r.player_one == pick::scissors && r.player_two == pick::rock)
        {
            return 6;
        }
        return 0;
    }(r);

    return static_cast<uint64_t>(r.player_two) + outcome;
}

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input {argv[1]};
    std::vector<round> const rounds {std::istream_iterator<round> {input}, {}};

    auto const total_score = std::accumulate(std::cbegin(rounds),
        std::cend(rounds),
        uint64_t {0},
        [](auto&& acc, auto&& r) { return acc + as_score(r); });

    std::cout << total_score << '\n';
}
