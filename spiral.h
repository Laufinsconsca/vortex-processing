#ifndef SPIRAL_H
#define SPIRAL_H
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <QLineEdit>
#include <complex>
#include "enums.h"
#include "transformation.h"

class spiral {
public:
    /** designed propagation along z-axis */
    double z;
    /** designed wavelength */
    double lambda;
    /** designed topological charge */
    double l;
    double r0;
    /** logarithmic spiral parameters*/
    double a;
    double b;
    /* spiral thickness */
    double d;
    double ph_size;
    enum spiral_type spiral_type;
    spiral();
    spiral(double z, double lambda, double l, double r0, double a, double b, double d, double ph_size, enum spiral_type spiral_type);
    static std::vector<std::vector<double>>& spiral_to_vector(const spiral& spiral, std::vector<std::vector<double>>& spiral_vector, const QSize& size);
    //static bool spiral_param_preprocessing(QLineEdit* z_line, QLineEdit* lambda_line, QLineEdit* l_line, QLineEdit* r0_line, QLineEdit* a_line, QLineEdit* b_line, QLineEdit* spiral_thickness_line, QLineEdit* ph_size_line, class spiral& spiral);
    static bool archimedean_spiral_def(const spiral& spiral, double r, double angle, double ph_size_coefficient);
    static bool logarithmic_spiral_def(const spiral& spiral, double r, double angle);
    spiral& operator=(const spiral& obj);
};

#endif // SPIRAL_H
