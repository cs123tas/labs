#ifndef SQUARE_H
#define SQUARE_H

#include "Shape.h"

class Square : public Shape
{
public:
    Square();
    //declaration of the destructor
    ~Square();

    int sideLength;

    void set_side_length(int);

    int area();

};

#endif // SQUARE_H
