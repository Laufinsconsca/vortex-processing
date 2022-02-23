#include "gauss_beam.h"

/**
 * Default constructor.
 **************************************************************************************************/

gauss_beam::gauss_beam() : sigma(0), shift(0), shift_angle(0) {}

gauss_beam::gauss_beam(double sigma, double shift, double shift_angle)
    : sigma(sigma), shift(shift), shift_angle(shift_angle), spiral_(spiral()) {}

gauss_beam::gauss_beam(double sigma, double shift, double shift_angle, class spiral spiral_)
    : sigma(sigma), shift(shift), shift_angle(shift_angle), spiral_(spiral_) {}


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
            if (gauss_beam.spiral_.lambda == 0) {
                ref_beam.at(i).push_back((exp(-(pow(x - gauss_beam.shift, 2) + y * y) / (2 * gauss_beam.sigma * gauss_beam.sigma))));
            } else {
                double r = sqrt(x*x+y*y);
                double angle = std::atan2(-y, x);
                double lambda = gauss_beam.spiral_.lambda/1e9;
                double ph_size_coefficient = 4./(gauss_beam.spiral_.ph_size*gauss_beam.spiral_.ph_size);
                if (abs(r*r - gauss_beam.spiral_.r0*gauss_beam.spiral_.r0 - ph_size_coefficient*angle*gauss_beam.spiral_.z*gauss_beam.spiral_.l*lambda/M_PI) < gauss_beam.spiral_.d) {
                    ref_beam.at(i).push_back((exp(-(pow(x - gauss_beam.shift, 2) + y * y) / (2 * gauss_beam.sigma * gauss_beam.sigma))));
                } else {
                    ref_beam.at(i).push_back(0);
                }
            }
        }
    }
    return transformation<std::vector<double>>::rotate(ref_beam, gauss_beam.shift_angle*M_PI/180);
}

std::vector<std::vector<double>>& gauss_beam::gauss_beam_to_vector(const gauss_beam& gauss_beam, std::vector<std::vector<double>>& ref_beam, const QSize& size, const hole &hole) {
    ref_beam = gauss_beam_to_vector(gauss_beam, ref_beam, size);
    hole::create_hole(ref_beam, hole, 1);
    return ref_beam;
}

bool gauss_beam::gauss_param_preprocessing(QLineEdit* sigma_line, QLineEdit* shift_line, QLineEdit* shift_angle_line, gauss_beam &gauss_beam) {
    if (sigma_line->text().isEmpty() || shift_line->text().isEmpty() || shift_angle_line->text().isEmpty()) {
        return false;
    }
    bool sigma_line_to_string_ok, shift_line_to_string_ok, shift_angle_line_to_string_ok;
    double sigma = sigma_line->text().toDouble(&sigma_line_to_string_ok);
    double shift = shift_line->text().toDouble(&shift_line_to_string_ok);
    double shift_angle = shift_angle_line->text().toDouble(&shift_angle_line_to_string_ok);
    if (shift_line_to_string_ok && sigma_line_to_string_ok && shift_angle_line_to_string_ok) {
        class gauss_beam result_gauss(sigma, shift, shift_angle);
        gauss_beam = result_gauss;
        return true;
    } else {
        return false;
    }
}

bool gauss_beam::spiral_gauss_param_preprocessing(QLineEdit* sigma_line, QLineEdit* shift_line, QLineEdit* shift_angle_line, QLineEdit* z_line, QLineEdit* lambda_line, QLineEdit* l_line, QLineEdit* r0_line, QLineEdit* spiral_thickness_line, QLineEdit* ph_size_line, gauss_beam &gauss_beam) {
    if (sigma_line->text().isEmpty() || shift_line->text().isEmpty() || shift_angle_line->text().isEmpty() || z_line->text().isEmpty() || lambda_line->text().isEmpty() || l_line->text().isEmpty() || r0_line->text().isEmpty() || spiral_thickness_line->text().isEmpty() || ph_size_line->text().isEmpty()) {
        return gauss_param_preprocessing(sigma_line, shift_line, shift_angle_line, gauss_beam);
    }
    bool sigma_line_to_string_ok, shift_line_to_string_ok, shift_angle_line_to_string_ok, z_line_to_string_ok, lambda_line_to_string_ok, l_line_to_string_ok, r0_line_to_string_ok, spiral_thickness_line_to_string_ok, ph_size_line_ok;
    double sigma = sigma_line->text().toDouble(&sigma_line_to_string_ok);
    double shift = shift_line->text().toDouble(&shift_line_to_string_ok);
    double shift_angle = shift_angle_line->text().toDouble(&shift_angle_line_to_string_ok);
    double z = z_line->text().toDouble(&z_line_to_string_ok);
    double lambda = lambda_line->text().toDouble(&lambda_line_to_string_ok);
    double l = l_line->text().toDouble(&l_line_to_string_ok);
    double r0 = r0_line->text().toDouble(&r0_line_to_string_ok);
    double d = spiral_thickness_line->text().toDouble(&spiral_thickness_line_to_string_ok);
    double ph_size = ph_size_line->text().toDouble(&ph_size_line_ok)/1000;
    if (shift_line_to_string_ok && sigma_line_to_string_ok && shift_angle_line_to_string_ok && z_line_to_string_ok && lambda_line_to_string_ok && l_line_to_string_ok && r0_line_to_string_ok && spiral_thickness_line_to_string_ok && ph_size_line_ok) {
        class gauss_beam result_gauss(sigma, shift, shift_angle, spiral(z, lambda, l, r0, d, ph_size));
        gauss_beam = result_gauss;
        return true;
    } else {
        return false;
    }
}

/**
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
    shift_angle = obj.shift_angle;
    spiral_ = obj.spiral_;
    return *this;
}
