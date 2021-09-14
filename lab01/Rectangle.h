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


    int area();

};

#endif // RECTANGLE_H
