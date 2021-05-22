#ifndef VORTEX_H
#define VORTEX_H

#include <QLineEdit>
#include <functional>
#include "hole.h"
#include <complex>

/************************************************************************************************//**
 * Declares the vortex class (this class contains a structure describing the vortex phase
 * according to its definition e^(i*tp(r)*(fi^n)),
 * tp(r) - topological charge (or a continuous function in the case of topological charge dependence on the radius),
 * n - power of fi)
 **************************************************************************************************/

/** A vortex. */

class vortex {
    /** The topological charge (is presented as functional object due to the functional dependence of the topological charge on the radius). */
    std::function<double(double)> f_tp;
    vortex& operator=(const vortex& obj);
public:
    /** The power of the azimuthal angle. */
    double pow_fi;
    vortex(std::function<double(double)> f, double pow_fi);
    vortex();
    vortex(double tp, double pow_fi);
    double tp(double r) const;
    static std::vector<std::vector<double>>& vortex_to_vector(const vortex& vortex, std::vector<std::vector<double>>& spp, const QSize& size);
    static std::vector<std::vector<double>>& vortex_to_vector(const vortex& vortex, std::vector<std::vector<double>>& spp, const QSize& size, const hole& hole);
    static bool spp_param_preprocessing(QLineEdit* m_line, QLineEdit* fi_line, class vortex& vortex);
};

#endif // VORTEX_H
