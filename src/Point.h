#ifndef POINT_H_
#define POINT_H_

#include <tuple>
#include <array>

class Point 
{
    public:
        Point();
        void setColour(unsigned int r, unsigned int g, unsigned int b);
        std::tuple<int, int, int> getColour() const { return _colour; };

    private:
      std::tuple<int, int, int> _colour;

};

#endif // POINT_H_