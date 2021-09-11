#include <iostream>
#include <memory>
#include "Rectangle.h"
#include "Square.h"


//basic increment funtion that is a void and
//increments the value that the pointer points to
void increment(int* j, int i)
{
    //  TODO (task 2): Using the pointer j, increment the
    //        value of the integer j points to by
    //        the amount i

}


//basic pointer to a local int (no need for delete statement)
void pointerFunctionInt()
{
    //creation of int and a pointer to that int
    int i = 149;
    int* j = &i;


    //printing the value stored at the pointer given to
    //the function increment before the funtion runs
    // NOTE! *j is the same as writing i
    std::cout << "before increment = " << *j << std::endl;

    //  TODO (task 3): Using the increment function and the pointer j,
    //        increment i by 1, from 149 to 150


    //printing the value stored at the pointer given to
    //the function increment after the funtion runs
    std::cout << "after increment = " << *j << std::endl;

}


//basic pointer to a local rectangle obj (delete statement 100% necessary)
void pointerFunctionRect()
{
    Rectangle* rec;
    //  TODO (task 8): Assign rec (a pointer to a rectangle object)
    //        to a new rectangle object



    int i = 10;
    int l = 10;

    //  TODO (task 9): Set the values of the rectangle rec to
    //        j and k as width and height respectively
    //        NOTE! Use an arrow instead of . as rec is
    //        a pointer to the object and not an object;
    //        If rec were an object member functions would
    //        be accessed by a .


    std::cout << "before height increment = " << rec->area() << std::endl;


    //  TODO (task 10): Using the increment function and the pointer rec->height,
    //        increment rec->height by 5, from 10 to 15


    std::cout << "after height increment = " << rec->area() << std::endl;


    // !!!very very important delete statement
    // that would cause memory leaks if not
    // included!!!

    //  TODO (task 11): Add delete statement


}


void smartPointers()
{
    //  TODO (task 16): Create a SMART! pointer to a Rectangle object


    //using two seperate ints and pointers to those ints
    //is will make the program act how you would expect as
    //if k = &i, when we increment height or width it would
    //also increase the other as we increase what those pointers
    //point to and that would be the same.
    int i = 10;

    //  TODO (task 17): Set the values of the square sqr to i
    //
    //        NOTE! Use an arrow instead of . as rec is
    //        a pointer to the object and not an object;
    //        If rec were an object member functions would
    //        be accessed by a .


    /* uncomment this after step 15
    std::cout << "before length increment = " << sqr->area() << std::endl;
    */

    //  TODO (task 18): Using the increment function and the pointer rec->height,
    //        increment rec->height by 5, from 10 to 15

    /* uncomment this after step 15
    std::cout << "after length increment = " << sqr->area() << std::endl;
    */



    // !!!No delete statement needed because of the use of smart pointer!!!
}


void twoDimArray()
{
    //this is an array where every object is an array
    //of dimension 10x10
    int j[10][10];
    int count = 0;

    for(int row = 0; row < 10; row++)
    {
        for(int col = 0; col < 10; col++)
        {
            j[row][col] = count;
            count++;
        }
    }

    int r = 2;
    int c = 4;

    //  TODO (task 19): Index into the 2D array and
    //        print the value at row r and col c

}


void oneDimArrayAsTwoDimArray()
{
    //this is an array where every object is an array
    //of dimension 10x10
    int j[100];


    for(int x = 0; x < 100; x++)
    {
        j[x] = x;
    }

    int width = 10;
    int r = 2;
    int c = 4;

    //  TODO (task 20): Index into the 1D array as if it was
    //        a 2D array and print the value at
    //        row r and col c


}


void inheritanceArray()
{
    //  TODO (task 21): Create an array of Shapes,
    //                 shapeArray, and fill it with
    //                 both rectangles and squares


    /* uncomment this after step 20
    Rectangle* a = new Rectangle;
    Square* b = new Square;
    Rectangle* c = new Rectangle;

    a->set_values(5,6);
    b->set_values(4);
    c->set_values(2,5);
    */

    //  TODO (task 22): Fill shapeArray with
    //                  a, b, and c




    for(int i = 0; i < 3; i++)
    {
        //  TODO (task 23): print out the area of each shape
        //                  in the array!
    }


    //  TODO (task 24): Since we used NEW key words
    //                 we must go and delete these
    //                 objects before they go out of scope

}



void helloWorld()
{
    //  TODO (task 1): Write Hello World in a
    //                 standard out call


}




int main()
{
    //  TODO (task 1-24): uncomment each function call
    //           as you fill out each function


//    helloWorld();
//    std::cout << std::endl;

//    pointerFunctionInt();
//    std::cout << std::endl;

//    pointerFunctionRect();
//    std::cout << std::endl;

//    smartPointers();
//    std::cout << std::endl;

//    twoDimArray();
//    std::cout << std::endl;

//    oneDimArrayAsTwoDimArray();
//    std::cout << std::endl;

//    inheritanceArray();
//    std::cout << std::endl;




    //return statement of main
    return 0;
}










