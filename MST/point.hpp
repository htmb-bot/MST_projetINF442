#pragma once // ensures that this header file is only included once

#include <string>

class point  // this is just a declaration of the class 
             // implementation is in a separate file: point.cpp
{
    static int d;

public:
    double *coords;
    int label;
    std::string etiquette;

    static bool set_dim(int _d);
    static int get_dim();

    point();
    ~point();

    void print();
    double dist(point &q);
};
