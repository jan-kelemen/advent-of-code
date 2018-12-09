#include <algorithm>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <list>
#include <memory_resource>
#include <vector>

#include <boost/container/adaptive_pool.hpp>

int main()
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto marbles = 0, players = 0;
    scanf("%d players; last marble is worth %d points", &players, &marbles);
    marbles *= 100;

    //auto playfield = std::list<int>{}; // ~470 ms
    //auto playfield = std::list<int, boost::container::adaptive_pool<int>>{}; // ~330 ms
    //auto resource = std::pmr::unsynchronized_pool_resource{}; // ~230 ms
    auto resource = std::pmr::monotonic_buffer_resource{}; // ~160 ms
    auto playfield = std::pmr::list<int>(&resource);
    playfield.push_back(0);

    auto scores = std::vector<int64_t>{};
    scores.resize(players);

    auto current_iterator = playfield.cbegin();
    for (auto marble = 1, player = 0; marble != marbles + 1; ++marble, player = (player + 1) % players)
    {
        if (marble % 23)
        {
            if (++current_iterator == playfield.cend())
                 current_iterator = playfield.cbegin();
            current_iterator = playfield.insert(++current_iterator, marble);
        }
        else
        {
            for (auto i = 0; i != 7; ++i, --current_iterator)
                 if (current_iterator == playfield.cbegin())
                     current_iterator = playfield.cend();

            scores[player] += marble + *current_iterator;
            playfield.erase(current_iterator++);
        }
    }

    std::cout << *std::max_element(scores.cbegin(), scores.cend()) << '\n';
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << '\n';
}
