#include "Heading.h"

#include <stdexcept>

Heading rotate(Heading const& current, Direction const& direction) {
    switch (current) {
        case Heading::North:
            return direction == Direction::Left ? Heading::West : Heading::East;
        case Heading::South:
            return direction == Direction::Left ? Heading::East : Heading::West;
        case Heading::East:
            return direction == Direction::Left ? Heading::North : Heading::South;
        case Heading::West:
            return direction == Direction::Left ? Heading::South : Heading::North;
        default:
            throw std::invalid_argument("Invalid heading");
    }
}
