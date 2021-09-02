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

    void set_values (int);

    int area()
    {
        //  TODO (task 12): Write a function that returns
        //        the area of square given its sideLength
    }

};

#endif // SQUARE_H
