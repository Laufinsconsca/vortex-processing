#include "polygonal_spiral.h"

polygonal_spiral::polygonal_spiral() : pitch(0), total(0), skip(0), d(0), polygonal_spiral_type(polygonal_spiral_type::square) {}

polygonal_spiral::polygonal_spiral(double pitch, double total, double skip, double d, enum polygonal_spiral_type polygonal_spiral_type) : pitch(pitch), total(total), skip(skip), d(d), polygonal_spiral_type(polygonal_spiral_type) {}

bool polygonal_spiral::line(const double r, const double angle, const double ph_size, const double d){
    return abs(r*cos(angle) - ph_size) < d;
}

double polygonal_spiral::pentagon_inner_radius(const double outer_radius){
    return outer_radius*sqrt(5+2*sqrt(5))/sqrt(10+2*sqrt(5));
}

double polygonal_spiral::pentagon_side(const double outer_radius){
    return outer_radius*sqrt(10)/sqrt(5+sqrt(5));
}

double polygonal_spiral::top_left_x(const double outer_radius){
    double q = pentagon_inner_radius(outer_radius) - pentagon_side(outer_radius)*sin(M_PI/10);
    return (outer_radius - q/tan(asin(q/outer_radius)))/2;
}

double polygonal_spiral::bottom_left_x(const double outer_radius){
    return top_left_x(outer_radius) + pentagon_side(outer_radius)*sin(M_PI/10);
}

double polygonal_spiral::top_left_y(const double outer_radius){
    return outer_radius-pentagon_inner_radius(outer_radius)+pentagon_side(outer_radius)*cos(M_PI/10);
}

/**********************************************************************************************//**
* Assignment operator
*
* @param	obj	The object to copy.
*
* @returns	A shallow copy of this object.
**************************************************************************************************/

class polygonal_spiral& polygonal_spiral::operator=(const polygonal_spiral &obj) {
    if (this == &obj) {
        return *this;
    }
    pitch = obj.pitch;
    skip = obj.skip;
    total = obj.total;
    d = obj.d;
    polygonal_spiral_type = obj.polygonal_spiral_type;
    return *this;
}
