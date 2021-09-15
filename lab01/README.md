# Lab 01 — C++ Introduction                         

<img src="https://www.pikpng.com/pngl/m/444-4446297_c-clipart-png-download.png" alt="drawing" width="200"/>

## Prologue

Today we will be learning the basics of the C++ language in order to assist with the rest of the assignments in the course. Throughout this lab you will work with memory management, debugging, and some simple pixel array processing. This lab will help you prepare for the future projects in the class where you will have to use all these elements in order to have stable, well written programs. Let's get started!

## Intro

C++ is a programming language that is syntactically similar to Java and C. However, it is very important to remember that it is its own thing and not simply "C with classes." There are a lot of things that will do exactly what you expect, but there are also many things that won't, so it is important to understand how things work behind the scenes. C++ is used in this class as it is a popular language for games, graphical user interfaces, and many other systems involved with graphics.

C++ was developed by Danish computer scientist Bjarne Stroustrup at Bell Labs in 1979 as an extension of the C language; he wanted an efficient and flexible language similar to C that also provided high-level features for program organization.

## Getting Started

If you have already cloned the lab repo locally during labs before, navigate into `~/course/cs1230/labs` (wherever you cloned the lab repository) and run the command 
```
git pull origin master
```
To get the most up to date version of lab01!

Otherwise you can get the labs starter code by opening up a terminal and running:
```
git clone https://github.com/cs123tas/labs.git
```
Open Qt Creator and select "Open Project" and find the `lab01.pro` file in your lab01 directory.

On the Configure Project screen, make sure Desktop is selected and select "Configure Project." Once everything is configured, go to the Projects menu on the left and make sure that the "Shadow build" option is **not** checked. In addition, you may want to uncheck "Run in terminal" (Build & Run -> Run -> Run -> Working directory -> Run in terminal). This will run our program in Qt Creator's terminal rather than opening your native one.

While there is also a demo version of the lab that can be run by inputting `cs1230_lab01_demo` into your terminal on the department machine through FastX3, there are images along the way that should be sufficient for this lab. See [this link](https://cs.brown.edu/about/system/connecting/fastx/) on how to access department machines through FastX3.

## A simple C++ program that does nothing

A simple hello world is provided below:
```cpp
int main() { // Program begins with the "main" function
  return 0; // Return 0 on success
}
```
In the same way that Java uses `import`s, C++ uses `#include`s. In the way that Java uses packages, C++ uses namespaces, which are denoted with the double colon `::` symbol. One namespace that is used in the Hello World is the C++ Standard Library, also known as _std_.

The **input/output library** (`iostream` library) is part of the C++ standard library that deals with basic input and output. We'll use the functionality in this library to get input from the keyboard and output data to the console. The _io_ part of _iostream_ stands for _input/output_.

- Available any time via `#include <iostream>`
- `std::cout` is how you will print to the command line
  - i.e. the output window in QtCreator, your IDE for this course :)

- `<<` inserts characters into the current output stream
- Works like string concatenation where the stream is the output string
- `std::endl` ends the line and inserts a newline character
- `std::cin` and `std::cerr` also exist
  - `std::cin` reads from stdin (terminal input)
  - `std::cerr` prints to stderr (terminal error messages)

#### ***Task 1:***

We are ready to write something on the screen!

- Write "Hello World!" in a `std::cout << "" << std::endl;` call in the `helloWorld` function and uncomment `helloWorld()` in `main()`.

At this point when you run your program you should see "Hello World!"

Now we can get into the more fun stuff!

## Pointers

- Variables exist in memory
- Pointers represent memory locations
- Pointers allow you to hold on to the _location_ of the object, rather than the object itself
- This is especially helpful when you need to allocate large objects and pass them around

#### ***Task 2:***

Navigate to the `increment()` function at the top of `main.cpp`. It takes two arguments: `j` and `i`. The type of `i` is an integer, represented by `int`. The type of `j` is a _pointer to an integer_, represented by `int*`.

We are going to use the pointer `j` to increment, by `i`, the _value of the integer that `j` points to_. To do this we must _dereference_ `j` by using an asterisk.
```
*j += i;
```

#### ***Task 3:***

In the function `pointerFunctionInt()`, we want to use the `increment()` function we just wrote and the pointer `j` to increment `i` by 1, from 149 to 150. Note that `i`'s location in memory is extracted with the `&` operator.

After you've done that, you should go down to the bottom `main()` function and uncomment out the `pointerFunctionInt()` call. When you run the program, you should see this output
```
before increment = 149
after increment = 150
```
If you see this, you know you have done the first function correctly!

Next, we will be using the `Rectangle` class which is a child of the `Shape` class. Take some time to look at `Rectangle.h` to see how inheritance works in C++.

#### ***Task 4:***

Now head on over to the file `Rectangle.cpp`, write a function that returns the area of a rectangle given its member variables `width` and `height`.

#### ***Task 5:***

Next we are going to write the definition of the _constructor_, which is run at the creation of a `Rectangle` object and is used to set the defaults of member variables and more.

- A basic constructor will have the form
```cpp
ClassName::ClassName() { /* constructor code */ }
```
- A cool thing about C++ is that we can efficiently set default values for member variables using the syntax below:
```c++
ClassName::ClassName() :
    memberVar1(defaultVal),
    memberVar2(defaultVal)
{
    /* constructor code */
}
```
This is called a member *initializer list*!

- In our case we will be setting `width` and `height` to zero.

#### ***Task 6:***

Write the definition of the _destructor_ now for the same class.

Destructors are run at the deletion of the object and are used to deallocate memory and do other cleanup tasks for a class object and its members when the object is destroyed.

- A basic destructor will have the form
```cpp
ClassName::~ClassName() { /* destructor code */ }
```
- Ours will simply print out the the string "RECTANGLE DESTRUCTOR". (Make sure to use double quotes! Single quotes represent something else in C++.)

A common use case for destructors is when you have a class that calls `new` in its constructor. When the object gets deleted, all the references to the object created in the constructor will be lost as well, so `delete` must be called in the destructor to free up that memory, preventing a _memory leak_.

#### ***Task 7:***

Write the definition to the member function `set_values()` such that it takes in two integers `w` and `h` then sets the member variables `width` and `height` accordingly.

Now heading back to `main.cpp`, in the `pointerFunctionRect()` function we can see a pointer to a `Rectangle` object has been created. This simply _allocated the space_ for a rectangle; there is not an actual rectangle object yet.

#### ***Task 8:***

Assign rec (a pointer to a rectangle object) to a NEW rectangle object using the line `rec = new Rectangle();`.

We now have a pointer to an existing rectangle object!

#### ***Task 9:***

Using the member function `set_values()`, set the width and height of the rectangle `rec` to `w` and `h` respectively. This can be done with `rec->set_values(w, h)`.

> NOTE! Here we use an _arrow_ `->` instead of `.` as `rec` is a _pointer_ to an object and not the object itself. If `rec` were an object, member functions would be accessed by a `.`;

#### ***Task 10:***

Using the `increment` function, increment `rec->height` by 5. Since the first parameter of `increment` expects a pointer to an integer, we should not pass in `rec->height`, but rather, the memory address of `rec->height`. Again, we can extract this with `&`.
```cpp
increment(&(rec->height), 5);
```

The next step is very important! In a language like C or C++ there is no automatic garbage collection. This means when the scope of our `rec` pointer ends, the memory that that object is taking up is not freed automatically. We must do this manually (unless we are _smart_ about it beforehand, more on that later)

#### ***Task 11:***

Free up the allocated memory by typing `delete rec;`.

- A good rule of thumb to help remember when you need to use a delete statement is whenever you use the keyword `new`, there _needs_ to be a corresponding `delete`.

At this point we can run our program! Go down to `main` and uncomment out the `pointerFunctionRect()` call. If you have done everything correctly you should see the output:
```
before height increment = 100
after height increment = 150
RECTANGLE DESTRUCTOR
```
from the function we just called, in addition to the prior ones we have done!

Now, moving on into our next function, we will be using the `Square` class which is a child of the `Shape` class as well. Take some time to look at `Square.h` to see how it _inherits_ from `Shape`.

#### ***Task 12:***

In `Square.cpp`, fill out the `area()` function that returns the area of a `Square` using only the member variable `sideLength`.

#### ***Task 13:***

Now write the definition of the constructor for the `Square` subclass. Make sure to set its member variable `sideLength` to zero in the member initializer list as we did in Task 5.

#### ***Task 14:***

Write the definition of the destructor the exact same way we wrote the one for the `Rectangle` class in Task 6, except this one should print out the string "SQUARE DESTRUCTOR".

#### ***Task 15:***

Fill out `set_side_length()`. This should take in an integer `x` and set the member variable `sideLength` to equal `x`.

## Smart Pointers

Moving back to our `main.cpp` file, we will be using SMART pointers!

In most cases, when you initialize a raw pointer or resource handle to point to an actual resource, you should pass the pointer to a smart pointer immediately. In modern C++, raw pointers are only used in small code blocks of limited scope, loops, or helper functions where performance is critical and there is no chance of confusion about ownership.

The following example compares a raw pointer declaration to a smart pointer declaration.
```cpp
#include <memory>

voidUseRawPointer()
{
  // Using a raw pointer -- not recommended.
  Song* pSong = new Song("Nothing on You", "Bruno Mars");
  // Use pSong...
  // Don't forget to delete!
  delete pSong;
}

voidUseSmartPointer()
{
  // Declare a smart pointer on stack and pass it the raw pointer.
  std::unique_ptr<Song> song2(new Song("Nothing on You", "Bruno Mars"));
  // Use song2…
  int d = song2->duration;
  // ...
} // song2 is deleted automatically here.
```

#### ***Task 16:***

In the function `smartPointers()`, create a smart pointer to a `Square` object using `std::unique_ptr<Square> sqr(new Square());`.


#### ***Task 17:***

Using `sqr->set_side_length()`, set the side length of the `Square` that `sqr` points at to `i`. As before, we use an arrow to access the actual `Square` object's member functions, rather than a `.`

#### ***Task 18:***

Using the `increment` function, increment `sqr->sideLength` by 5 by giving our increment function the address of that variable, as before.
```cpp
increment(&(sqr->sideLength), 5);
```
Then uncomment the statements that print out the area of `sqr`.

Go down to the bottom main function and uncomment out the `smartPointers()` call. If you have done everything correctly, you should see the output:
```
before length increment = 100
after length increment = 225
SQUARE DESTRUCTOR
```
in addition to the prior output.

## Multidimensional Arrays

In the next two functions we will be using a 2 dimensional array. This data structure is useful in computer graphics as it is a good way to represent the pixel array on a display. One way to create this grid-like data structure is to use a 1D array (a built in data structure in C++) and fill it with other 1D arrays.

![Multidimensional array](https://cdn.journaldev.com/wp-content/uploads/2020/03/2D-array-representation.png)

This method allows us to index into the structure with a coordinate system that is intuitive.

If you wanted the object in the 2D array `arr` in the second row and fourth column you would index into it with `arr[1][3]`, keeping in mind that arrays are zero-indexed in C++.

***Task 19:***

Index into the 2D array `j` and print the value at row `r` and col `c`.

At this point we can run our program!!! Go down to the bottom main function and uncomment out the `twoDimArray()` call. If you have done everything correctly you should see the output:
```
24
```
...in addition to the prior output.

Another way to create a 2D array data structure is to use a 1D array and index into it in a way that it behaves _as if it were_ a two dimensional array. There are two ways to do this, **row-major** and **column-major**, like so:

![Row-major vs. column-major indexing](https://craftofcoding.files.wordpress.com/2017/02/rowcolumnarrays.jpg)

We will use row-major order. To do this we will use `j[width * r + c]`, where `width` is the number of columns and (r, c) is the same indexing system used in the previous example!

#### ***Task 20:***

Index into the 1D array as if it were a 2D array and print the value at row `r` and col `c`.

At this point we can run our program!!! Go down to the bottom main function and uncomment out the `oneDimArrayAsTwoDimArray()` call. If you have done everything correctly you should see the same output,
```
24
```

#### ***Task 21:***

In `inheritanceArray()`, create an array of pointers to `Shape`s of size 3 with the following:
```cpp
Shape* shapeArray[3];
```

#### ***Task 22:***

Fill `shapeArray` with `a`, `b`, and `c`.

#### ***Task 23:***

Print out the area of all the objects in the list of shapes in the for loop!

#### ***Task 24:***
Remember, Since we used `new` keywords we must `delete` these objects before they go out of scope! Delete them before the function ends.

At this point we can run our program!!! Go down to the bottom main function and uncomment out the `inheritanceArray()` call. If you have done everything correctly you should see the output:
```
30
16
10
RECTANGLE DESTRUCTOR
SQUARE DESTRUCTOR
RECTANGLE DESTRUCTOR
```

## End

Congratulations! You have finished your first C++ lab. We encourage you to use the style and techniques you learned in this lab.

Now you are ready to show your program to a TA to get checked off!
