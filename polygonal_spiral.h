#ifndef POLYGONAL_SPIRAL_H
#define POLYGONAL_SPIRAL_H
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <QLineEdit>
#include <complex>
#include "transformation.h"
#include "enums.h"
#include <iostream>

class polygonal_spiral {
public:
    double pitch;
    double total;
    double skip;
    /* spiral thickness */
    double d;
    enum polygonal_spiral_type polygonal_spiral_type;
    polygonal_spiral();
    polygonal_spiral(double pitch, double total, double skip, double d, enum polygonal_spiral_type polygonal_spiral_type);
    static bool line(const double r, const double angle, const double ph_size, const double d);
    static double pentagon_inner_radius(const double outer_radius);
    static double pentagon_side(const double outer_radius);
    static double top_left_x(const double outer_radius);
    static double bottom_left_x(const double outer_radius);
    static double top_left_y(const double outer_radius);
    polygonal_spiral& operator=(const polygonal_spiral& obj);
};

#endif // POLYGONAL_SPIRAL_H
