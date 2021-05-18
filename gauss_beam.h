#ifndef GAUSS_BEAM_H
#define GAUSS_BEAM_H
#include <vector>
#include <math.h>
#include "hole.h"

class gauss_beam {
public:
    double sigma;
    double shift;
    gauss_beam(double sigma, double shift);
    static std::vector<std::vector<double>>& gauss_beam_to_vector(const gauss_beam gauss_beam, std::vector<std::vector<double>>& ref_beam, const QSize& size);
    static std::vector<std::vector<double>>& gauss_beam_to_vector(const gauss_beam gauss_beam, std::vector<std::vector<double>>& ref_beam, const QSize& size, const hole& hole);
};

#endif // GAUSS_BEAM_H
