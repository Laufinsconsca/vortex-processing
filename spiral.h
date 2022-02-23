#ifndef SPIRAL_H
#define SPIRAL_H
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <QLineEdit>
#include <complex>
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
    /* spiral thickness */
    double d;
    double ph_size;
    spiral();
    spiral(double z, double lambda, double l, double r0, double d, double ph_size);
    static std::vector<std::vector<double>>& spiral_to_vector(const spiral& spiral, std::vector<std::vector<double>>& spiral_vector, const QSize& size);
    static bool spiral_param_preprocessing(QLineEdit* z_line, QLineEdit* lambda_line, QLineEdit* l_line, QLineEdit* r0_line, QLineEdit* spiral_thickness_line, QLineEdit* ph_size_line, class spiral& spiral);
    spiral& operator=(const spiral& obj);
};

#endif // SPIRAL_H
