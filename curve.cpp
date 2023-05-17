#include "curve.h"

curve::curve() : a(0), b(0), m(0), n1(0), n2(0), n3(0), d(0), ph_size(0) {}

curve::curve(double a, double b, double m, double n1, double n2, double n3, double d, double ph_size) : a(a), b(b), m(m), n1(n1), n2(n2), n3(n3), d(d), ph_size(ph_size) {}

std::vector<std::vector<double>>& curve::curve_to_vector(const curve& curve, std::vector<std::vector<double>>& curve_vector, const QSize& size) {
    double hx, hy, x, y;
    hx = 2. / size.width();
    hy = 2. / size.height();
    curve_vector.reserve(size.height());
    for (int i = 0; i < size.height(); i++) {
        y = i * hy - 1;
        curve_vector.push_back(std::vector<double>());
        curve_vector.at(i).reserve(size.width());
        for (int j = 0; j < size.width(); j++) {
            x = j * hx - 1;
            double r = sqrt(x*x+y*y);
            double angle = std::atan2(-y, x);
            if (curve_def_condition(r, angle, curve)) {
                curve_vector.at(i).push_back(M_PI);
            } else {
                curve_vector.at(i).push_back(0);
            }
        }
    }
    return curve_vector;
    //return transformation<std::vector<double>>::rotate(spiral_vector, M_PI);
}

bool curve::curve_def_condition(const double r, const double angle, const curve& curve){
    return abs(r - exp(0.2*angle)*curve.ph_size*pow(pow(abs(cos(curve.m*angle/4)/curve.a),curve.n2)+pow(abs(sin(curve.m*angle/4)/curve.b),curve.n3),-1/curve.n1)) < curve.d;
}

bool curve::square(const double r, const double angle, const curve& curve){
    return abs(r - 2*curve.ph_size/sqrt(2+sqrt(2+2*cos(4*angle)))) < curve.d;
}

//bool curve::line(const double r, const double angle, const double ph_size, const double d){
//    return abs(r*cos(angle) - ph_size) < d;
//}

//double curve::pentagon_inner_radius(const double outer_radius){
//    return outer_radius*sqrt(5+2*sqrt(5))/sqrt(10+2*sqrt(5));
//}

//double curve::pentagon_side(const double outer_radius){
//    return outer_radius*sqrt(10)/sqrt(5+sqrt(5));
//}

//double curve::top_left_x(const double outer_radius){
//    double q = pentagon_inner_radius(outer_radius) - pentagon_side(outer_radius)*sin(M_PI/10);
//    return (outer_radius - q/tan(asin(q/outer_radius)))/2;
//}

//double curve::bottom_left_x(const double outer_radius){
//    return top_left_x(outer_radius) + pentagon_side(outer_radius)*sin(M_PI/10);
//}

//double curve::top_left_y(const double outer_radius){
//    return outer_radius-pentagon_inner_radius(outer_radius)+pentagon_side(outer_radius)*cos(M_PI/10);
//}

bool curve::curve_param_preprocessing(QLineEdit* a_line, QLineEdit* b_line, QLineEdit* m_line, QLineEdit* n1_line, QLineEdit* n2_line, QLineEdit* n3_line, QLineEdit* curve_thickness_line, QLineEdit* ph_size_line, class curve& curve) {
    if (a_line->text().isEmpty() || b_line->text().isEmpty() || m_line->text().isEmpty() || n1_line->text().isEmpty() || n2_line->text().isEmpty() || n3_line->text().isEmpty() || curve_thickness_line->text().isEmpty() || ph_size_line->text().isEmpty()) {
        return false;
    }
    bool a_line_to_string_ok, b_line_to_string_ok, m_line_to_string_ok, n1_line_to_string_ok, n2_line_to_string_ok, n3_line_to_string_ok, curve_thickness_line_to_string_ok, ph_size_line_ok;
    double a = a_line->text().toDouble(&a_line_to_string_ok);
    double b = b_line->text().toDouble(&b_line_to_string_ok);
    double m = m_line->text().toDouble(&m_line_to_string_ok);
    double n1 = n1_line->text().toDouble(&n1_line_to_string_ok);
    double n2 = n2_line->text().toDouble(&n2_line_to_string_ok);
    double n3 = n3_line->text().toDouble(&n3_line_to_string_ok);
    double d = curve_thickness_line->text().toDouble(&curve_thickness_line_to_string_ok);
    double ph_size = ph_size_line->text().toDouble(&ph_size_line_ok);
    if (a_line_to_string_ok && b_line_to_string_ok && m_line_to_string_ok && n1_line_to_string_ok && n2_line_to_string_ok && n3_line_to_string_ok && curve_thickness_line_to_string_ok && ph_size_line_ok) {
        class curve a_curve(a, b, m, n1, n2, n3, d, ph_size);
        curve = a_curve;
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

class curve& curve::operator=(const curve &obj) {
    if (this == &obj) {
        return *this;
    }
    a = obj.a;
    b = obj.b;
    m = obj.m;
    n1 = obj.n1;
    n2 = obj.n2;
    n3 = obj.n3;
    d = obj.d;
    ph_size = obj.ph_size;
    return *this;
}
