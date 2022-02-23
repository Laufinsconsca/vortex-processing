#include "spiral.h"

spiral::spiral() : z(0), lambda(0), l(0), r0(0), d(0), ph_size(0) {}

spiral::spiral(double z, double lambda, double l, double r0, double d, double ph_size) : z(z), lambda(lambda), l(l), r0(r0), d(d), ph_size(ph_size) {}

std::vector<std::vector<double>>& spiral::spiral_to_vector(const spiral& spiral, std::vector<std::vector<double>>& spiral_vector, const QSize& size) {
    double hx, hy, x, y;
    hx = 2. / size.width();
    hy = 2. / size.height();
    spiral_vector.reserve(size.height());
    for (int i = 0; i < size.height(); i++) {
        y = i * hy - 1;
        spiral_vector.push_back(std::vector<double>());
        spiral_vector.at(i).reserve(size.width());
        for (int j = 0; j < size.width(); j++) {
            x = j * hx - 1;
            double r = sqrt(x*x+y*y);
            double angle = std::atan2(-y, x);
            double lambda = spiral.lambda/1e9;
            double ph_size_coefficient = 4./(spiral.ph_size*spiral.ph_size);
            if (abs(r*r - spiral.r0*spiral.r0 - ph_size_coefficient*angle*spiral.z*spiral.l*lambda/M_PI) < spiral.d) {
                spiral_vector.at(i).push_back(M_PI);
                //spiral_vector.at(i).push_back(0);
            } else {
                spiral_vector.at(i).push_back(0);
            }
        }
    }
    return spiral_vector;
    //return transformation<std::vector<double>>::rotate(spiral_vector, M_PI);
}

bool spiral::spiral_param_preprocessing(QLineEdit* z_line, QLineEdit* lambda_line, QLineEdit* l_line, QLineEdit* r0_line, QLineEdit* spiral_thickness_line, QLineEdit* ph_size_line, class spiral& spiral) {
    if (z_line->text().isEmpty() || lambda_line->text().isEmpty() || l_line->text().isEmpty() || r0_line->text().isEmpty() || spiral_thickness_line->text().isEmpty() || ph_size_line->text().isEmpty()) {
        return false;
    }
    bool z_line_to_string_ok, lambda_line_to_string_ok, l_line_to_string_ok, r0_line_to_string_ok, spiral_thickness_line_to_string_ok, ph_size_line_ok;
    double z = z_line->text().toDouble(&z_line_to_string_ok);
    double lambda = lambda_line->text().toDouble(&lambda_line_to_string_ok);
    double l = l_line->text().toDouble(&l_line_to_string_ok);
    double r0 = r0_line->text().toDouble(&r0_line_to_string_ok);
    double d = spiral_thickness_line->text().toDouble(&spiral_thickness_line_to_string_ok);
    double ph_size = ph_size_line->text().toDouble(&ph_size_line_ok)/1000;
    if (z_line_to_string_ok && lambda_line_to_string_ok && l_line_to_string_ok && r0_line_to_string_ok && spiral_thickness_line_to_string_ok && ph_size_line_ok) {
        class spiral a_spiral(z, lambda, l, r0, d, ph_size);
        spiral = a_spiral;
        return true;
    } else {
        return false;
    }
}

/**********************************************************************************************//**
* Assignment operator
*
* @param	obj	The object to copy.
*
* @returns	A shallow copy of this object.
**************************************************************************************************/

class spiral& spiral::operator=(const spiral &obj) {
    if (this == &obj) {
        return *this;
    }
    z = obj.z;
    lambda = obj.lambda;
    l = obj.l;
    r0 = obj.r0;
    d = obj.d;
    ph_size = obj.ph_size;
    return *this;
}
