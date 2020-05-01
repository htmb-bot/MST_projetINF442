#include "cloud.hpp" // The header for the class implemented here
#include "point.hpp" // Used in all methods

#include <cassert> // This provides the assert() method

cloud::cloud(int _d, int _nmax)
{
    point::set_dim(_d);

    nmax = _nmax;
    n = 0;

    points = new point[nmax];
}

cloud::~cloud()
{
    delete[] points;
}

int cloud::get_n()
{
    return n;
}

point &cloud::get_point(int i)
{
    return points[i];
}

void cloud::add_point(point &p)
{
    assert(n < nmax);

    for (int m = 0; m < point::get_dim(); m++)
    {
        points[n].coords[m] = p.coords[m];
        points[n].etiquette = p.etiquette;
    }

    n++;
}

void cloud::load(std::ifstream &is)
{
    assert(is.is_open());

    // point to read into
    point p;
    p.label = 0;

    // while not at end of file
    while (is.peek() != EOF)
    {
        // read new points
        for (int m = 0; m < point::get_dim(); m++)
        {
            is >> p.coords[m];
        }

        // read ground-truth labels
        // unused in normal operation
        is >> p.etiquette;
        add_point(p);

        // consume \n
        is.get();
    }
}
