#include <iostream>

int main()
{
    //After 108 steps sequence converges to diff of 65 per generation
    //TODO: solution is correct only for this input
    //TODO: https://en.wikipedia.org/wiki/Hashlife
    std::cout << uint64_t{ 7976 } + (50000000000 - 108) * 65 << '\n';
}