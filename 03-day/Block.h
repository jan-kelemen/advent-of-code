#ifndef BLOCK_H

#define BLOCK_H

#include <map>

using coordinate_t = std::pair<uint32_t, uint32_t>;

class Block
{
public:
    Block() = default;

    void leave_present(const coordinate_t &coordinate);

    size_t visited_houses() const { return left_presents.size(); }

    ~Block() = default;

private:
    std::map<coordinate_t, uint32_t> left_presents;

};

#endif // BLOCK_H