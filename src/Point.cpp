#include <tuple>

#include "Point.h"

Point::Point()
{
    _colour = {0, 0, 0};
}

void Point::setColour(unsigned int r, unsigned int g, unsigned int b)
{
    _colour = {r, g, b};
}
