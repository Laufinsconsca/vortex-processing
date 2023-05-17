#ifndef CURVE_H
#define CURVE_H
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <QLineEdit>
#include <complex>
#include "transformation.h"
#include <iostream>

class curve {
public:
    double a;
    double b;
    double m;
    double n1;
    double n2;
    double n3;
    /* spiral thickness */
    double d;
    double ph_size;
    curve();
    curve(double a, double b, double m, double n1, double n2, double n3, double d, double ph_size);
    static std::vector<std::vector<double>>& curve_to_vector(const curve& curve, std::vector<std::vector<double>>& curve_vector, const QSize& size);
    static bool curve_param_preprocessing(QLineEdit* a_line, QLineEdit* b_line, QLineEdit* m_line, QLineEdit* n1_line, QLineEdit* n2_line, QLineEdit* n3_line, QLineEdit* curve_thickness_line, QLineEdit* ph_size_line, class curve& curve);
    static bool curve_def_condition(const double r, const double angle, const curve& curve);
    static bool square(const double r, const double angle, const curve& curve);
//    static bool line(const double r, const double angle, const double ph_size, const double d);
//    static double pentagon_inner_radius(const double outer_radius);
//    static double pentagon_side(const double outer_radius);
//    static double top_left_x(const double outer_radius);
//    static double bottom_left_x(const double outer_radius);
//    static double top_left_y(const double outer_radius);
    curve& operator=(const curve& obj);
};

#endif // CURVE_H
