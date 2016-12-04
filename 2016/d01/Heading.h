#ifndef HEADING_H
#define HEADING_H

#include "Direction.h"

enum class Heading {
    North, West, South, East
};

Heading rotate(Heading const& current, Direction const& direction);

#endif // !HEADING_H

