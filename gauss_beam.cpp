#include "gauss_beam.h"

gauss_beam::gauss_beam(double sigma, double shift)
    : sigma(sigma), shift(shift) {}

std::vector<std::vector<double>>& gauss_beam::gauss_beam_to_vector(const gauss_beam gauss_beam, std::vector<std::vector<double>>& ref_beam, const QSize& size) {
    double hx, hy, x, y;
    hx = 2. / size.width();
    hy = 2. / size.height();
    ref_beam.reserve(size.height());
    for (int i = 0; i < size.height(); i++) {
        y = i * hy - 1;
        ref_beam.push_back(std::vector<double>());
        ref_beam.at(i).reserve(size.width());
        for (int j = 0; j < size.width(); j++) {
            x = j * hx - 1;
            ref_beam.at(i).push_back((exp(-(pow(x - gauss_beam.shift, 2) + y * y) / (2 * gauss_beam.sigma * gauss_beam.sigma))));
        }
    }
    return ref_beam;
}

std::vector<std::vector<double>>& gauss_beam::gauss_beam_to_vector(const gauss_beam gauss_beam, std::vector<std::vector<double>>& ref_beam, const QSize& size, const hole &hole) {
    gauss_beam_to_vector(gauss_beam, ref_beam, size);
    hole::create_hole(ref_beam, hole, 1);
    return ref_beam;
}
