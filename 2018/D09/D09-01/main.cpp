#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

int main()
{
    auto marbles = 0, players = 0;
    scanf("%d players; last marble is worth %d points", &players, &marbles);

    auto playfield = std::vector<int>{};
    playfield.reserve(marbles + 1);
    playfield.push_back(0);

    auto scores = std::vector<int>{};
    scores.resize(players);

    auto current_index = std::size_t{ 0 };
    for(auto marble = 1, player = 0; marble != marbles + 1; ++marble, player = (player + 1) % players)
    {
        if(marble % 23 != 0)
        {
            if (++current_index == playfield.size())
                current_index = 0;
            playfield.insert(playfield.cbegin() + ++current_index, marble);
        }
        else
        {
            for (auto i = 0; i != 7; ++i, --current_index)
                if (current_index == 0)
                    current_index = playfield.size();

            scores[player] += marble + playfield[current_index];
            playfield.erase(playfield.cbegin() + current_index);
        }
    }
    std::cout << *std::max_element(scores.cbegin(), scores.cend()) << '\n';
}
