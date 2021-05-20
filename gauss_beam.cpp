#include "gauss_beam.h"

gauss_beam::gauss_beam(double sigma, double shift)
    : sigma(sigma), shift(shift) {}

std::vector<std::vector<double>>& gauss_beam::gauss_beam_to_vector(const gauss_beam& gauss_beam, std::vector<std::vector<double>>& ref_beam, const QSize& size) {
    ref_beam.clear();
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

std::vector<std::vector<double>>& gauss_beam::gauss_beam_to_vector(const gauss_beam& gauss_beam, std::vector<std::vector<double>>& ref_beam, const QSize& size, const hole &hole) {
    ref_beam = gauss_beam_to_vector(gauss_beam, ref_beam, size);
    hole::create_hole(ref_beam, hole, 1);
    return ref_beam;
}

bool gauss_beam::gauss_param_preprocessing(QLineEdit* sigma_line, QLineEdit* shift_line, gauss_beam &gauss_beam) {
    if (sigma_line->text().isEmpty() || shift_line->text().isEmpty()) {
        return false;
    }
    bool shift_line_to_string_ok, sigma_line_to_string_ok;
    double sigma = sigma_line->text().toDouble(&shift_line_to_string_ok);
    double shift = shift_line->text().toDouble(&sigma_line_to_string_ok);
    if (shift_line_to_string_ok && sigma_line_to_string_ok) {
        class gauss_beam result_gauss(sigma, shift);
        gauss_beam = result_gauss;
        return true;
    } else {
        return false;
    }
}

/**********************************************************************************************//**
 * Assignment operator.
 *
 * @param	obj	The object to copy.
 *
 * @returns	A shallow copy of this object.
 **************************************************************************************************/

class gauss_beam& gauss_beam::operator=(const gauss_beam &obj) {
    if (this == &obj) {
        return *this;
    }
    sigma = obj.sigma;
    shift = obj.shift;
    return *this;
}

