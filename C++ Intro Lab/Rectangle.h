#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"
#include <iostream>

//basic rectangle class
class Rectangle : public Shape
{

  public:

    //declaration of the constructor
    Rectangle();
    //declaration of the destructor
    ~Rectangle();

    int width, height;


    //declaration of the funtion set_values
    void set_values(int, int);

    //declaration and definition of area
    //function which retruns the area of
    // the rectangle
    int area()
    {
        //  TODO (task 4): Write a function that returns
        //        the area of rectangle given width
        //        and height
    }
};

#endif // RECTANGLE_H
