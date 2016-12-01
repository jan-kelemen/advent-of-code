#include <iostream>
#include <memory>
#include <sstream>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "md5.h"
#include "hex.h"

using suffix_t = uint32_t;

std::string message(const std::string &prefix, const suffix_t &suffix)
{
    auto stream = std::ostringstream();
    stream << prefix << suffix;
    return stream.str();
}

std::string hash(std::string &message)
{
    auto hash = CryptoPP::Weak1::MD5();
    byte digest[CryptoPP::Weak1::MD5::DIGESTSIZE];

    hash.CalculateDigest(digest, reinterpret_cast<byte*>(const_cast<char*>(message.c_str())), message.length());

    auto encoder = std::make_unique<CryptoPP::HexEncoder>();

    auto output = std::string();
    encoder->Attach(new CryptoPP::StringSink(output));
    encoder->Put(digest, sizeof(digest));
    encoder->MessageEnd();

    return output;
}

bool starts_with(const std::string &hash, const std::string &pattern)
{
    return hash.compare(0, pattern.length(), pattern) == 0;
}

int with_leading_n(const std::string &key, const size_t n = 5)
{
    auto suffix = suffix_t(1);
    auto pattern = std::string(n, '0');

    while (!starts_with(hash(message(key, suffix)), pattern))
    {
        ++suffix;
    }

    return suffix;
}
int main()
{
    std::cout << with_leading_n("bgvyzdsv") << "\n";
    std::cout << with_leading_n("bgvyzdsv", 6) << "\n";
    return 0;
}