#ifndef GAUSS_BEAM_H
#define GAUSS_BEAM_H

#include <QLineEdit>
#include <vector>
#include <math.h>
#include "hole.h"

class gauss_beam {
public:
    double sigma;
    double shift;
    gauss_beam(double sigma, double shift);
    static std::vector<std::vector<double>>& gauss_beam_to_vector(const gauss_beam& gauss_beam, std::vector<std::vector<double>>& ref_beam, const QSize& size);
    static std::vector<std::vector<double>>& gauss_beam_to_vector(const gauss_beam& gauss_beam, std::vector<std::vector<double>>& ref_beam, const QSize& size, const hole& hole);
    static bool gauss_param_preprocessing(QLineEdit* sigma_label, QLineEdit* shift_label, gauss_beam& gauss_beam);
    class gauss_beam& operator=(const gauss_beam &obj);
};

#endif // GAUSS_BEAM_H
