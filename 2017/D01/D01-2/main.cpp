#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using captcha_t = std::vector<int8_t>;

captcha_t read_captcha(std::istream& stream);

int accumulate_consecutive_digits(captcha_t const& captcha);

int main(int argc, char** argv)
{
    auto captcha = read_captcha(std::cin);
    auto sum = accumulate_consecutive_digits(captcha);
    std::cout << sum << '\n';
}


captcha_t read_captcha(std::istream& stream)
{
    std::string captcha_string;
    std::getline(stream, captcha_string);

    auto rv = captcha_t();
    rv.reserve(captcha_string.length());
    std::transform(captcha_string.begin(), captcha_string.end(),
                   std::back_inserter(rv),
                   [](char c) { return c - '0'; });

    return rv;
}

int accumulate_consecutive_digits(captcha_t const& captcha)
{
    auto sum = 0;
    auto const size = captcha.size();
    auto const half = size / 2;
    for(size_t i = 0; i != size; ++i)
    {
        if(captcha[i] == captcha[(i + half) % size])
        {
            sum += captcha[i];
        }
    }
    return sum;
}
