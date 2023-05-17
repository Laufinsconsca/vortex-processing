#include "gauss_beam.h"

/**
 * Default constructor.
 **************************************************************************************************/

gauss_beam::gauss_beam() : sigma(0), shift(0), shift_angle(0) {type = amplitude_type::gauss_type;}

gauss_beam::gauss_beam(double sigma, double shift, double shift_angle)
    : sigma(sigma), shift(shift), shift_angle(shift_angle), spiral_(spiral()) {
    type = amplitude_type::gauss_type;
}

gauss_beam::gauss_beam(double sigma, double shift, double shift_angle, class spiral spiral_)
    : sigma(sigma), shift(shift), shift_angle(shift_angle), spiral_(spiral_) {
    type = amplitude_type::spiral_type;
}

gauss_beam::gauss_beam(double sigma, double shift, double shift_angle, class polygonal_spiral polygonal_spiral_)
    : sigma(sigma), shift(shift), shift_angle(shift_angle), polygonal_spiral_(polygonal_spiral_) {
    type = amplitude_type::polygonal_spiral_type;
}

gauss_beam::gauss_beam(double sigma, double shift, double shift_angle, class curve curve_)
    : sigma(sigma), shift(shift), shift_angle(shift_angle), curve_(curve_) {
    type = amplitude_type::curve_type;
}


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
            double r = sqrt(x*x+y*y);
            double angle = std::atan2(-y, x);
            double ph_size_coefficient = 4./(gauss_beam.spiral_.ph_size*gauss_beam.spiral_.ph_size);
            double pitch = gauss_beam.polygonal_spiral_.pitch;
            double polygonal_spiral_thickness = gauss_beam.polygonal_spiral_.d;
            bool squareSpiral1, squareSpiral2 = false;
            switch (gauss_beam.type) {
            case amplitude_type::gauss_type :
                ref_beam.at(i).push_back((exp(-(pow(x - gauss_beam.shift, 2) + y * y) / (2 * gauss_beam.sigma * gauss_beam.sigma))));
                break;
            case amplitude_type::polygonal_spiral_type :
                if (angle < 0) {
                    angle = angle + 2*M_PI;
                }
                switch (gauss_beam.polygonal_spiral_.polygonal_spiral_type) {
                case polygonal_spiral_type::triangular :
                    squareSpiral1 = (polygonal_spiral::line(r, angle+M_PI+M_PI/6, 0.5, polygonal_spiral_thickness)
                                && i < 0.75*size.height() && i > 0) //левая1
                            || (polygonal_spiral::line(r, angle-M_PI/6, 0.5, polygonal_spiral_thickness)
                                && i < 0.75*size.height() && i > 0);  //правая1
                    for (int p = 0; p < gauss_beam.polygonal_spiral_.total; p++) {
                        if (p < gauss_beam.polygonal_spiral_.skip) {
                            continue;
                        }
                        squareSpiral2 = squareSpiral2 || (polygonal_spiral::line(r, angle+M_PI/2, 0.5-p*pitch*2, polygonal_spiral_thickness)
                                    && j < (0.5+sqrt(3)/4-p*pitch*sqrt(3))*size.width() && j > (0.5-sqrt(3)/4+(p+1)*2*pitch/sqrt(3)+p*pitch/sqrt(3))*size.width()) //нижняя1
                                || (polygonal_spiral::line(r, angle+M_PI+M_PI/6, 0.5-(p+1)*pitch*2, polygonal_spiral_thickness)
                                    && i < (0.75-p*pitch)*size.height() && i > 2*(p+1)*pitch*size.height()) //левая2
                                || (polygonal_spiral::line(r, angle-M_PI/6, 0.5-(p+1)*pitch*2, polygonal_spiral_thickness)
                                    && i < (0.75-(p+1)*pitch)*size.height() && i > 2*(p+1)*pitch*size.height());  //правая2
                    }
                    break;
                case polygonal_spiral_type::square :
                    squareSpiral1 = (polygonal_spiral::line(r, angle+M_PI, sqrt(2)/2, polygonal_spiral_thickness)
                                && i < (0.5+sqrt(2)/4)*size.height() && i > (0.5-sqrt(2)/4)*size.height()) //левая1
                            || (polygonal_spiral::line(r, angle+3*M_PI/2, sqrt(2)/2, polygonal_spiral_thickness)
                                && j < (0.5+sqrt(2)/4)*size.width() && j > (0.5-sqrt(2)/4)*size.width()) // верхняя1
                            || (polygonal_spiral::line(r, angle, sqrt(2)/2, polygonal_spiral_thickness)
                                && i < (0.5+sqrt(2)/4)*size.height() && i > (0.5-sqrt(2)/4)*size.height());  //правая1
                    for (int p = 0; p < gauss_beam.polygonal_spiral_.total; p++) {
                        if (p < gauss_beam.polygonal_spiral_.skip) {
                            continue;
                        }
                        squareSpiral2 = squareSpiral2 || (polygonal_spiral::line(r, angle+M_PI/2, sqrt(2)/2-p*pitch*2, polygonal_spiral_thickness)
                                    && j < (0.5+sqrt(2)/4-p*pitch)*size.width() && j > (0.5-sqrt(2)/4+(p+1)*pitch)*size.width()) //нижняя1
                                || (polygonal_spiral::line(r, angle+M_PI, sqrt(2)/2-(p+1)*pitch*2, polygonal_spiral_thickness)
                                    && i < (0.5+sqrt(2)/4-p*pitch)*size.height() && i > (0.5-sqrt(2)/4+(p+1)*pitch)*size.height()) //левая2
                                || (polygonal_spiral::line(r, angle+3*M_PI/2, sqrt(2)/2-(p+1)*pitch*2, polygonal_spiral_thickness)
                                    && j < (0.5+sqrt(2)/4-(p+1)*pitch)*size.width() && j > (0.5-sqrt(2)/4+(p+1)*pitch)*size.width()) // верхняя2
                                || (polygonal_spiral::line(r, angle, sqrt(2)/2-(p+1)*pitch*2, polygonal_spiral_thickness)
                                    && i < (0.5+sqrt(2)/4-(p+1)*pitch)*size.height() && i > (0.5-sqrt(2)/4+(p+1)*pitch)*size.height());  //правая2
                    }
                    break;
                case polygonal_spiral_type::pentagonal :
                    squareSpiral1 = (polygonal_spiral::line(r, angle+M_PI-M_PI/10, polygonal_spiral::pentagon_inner_radius(1), polygonal_spiral_thickness)
                                && i < (0.5+polygonal_spiral::pentagon_inner_radius(0.5))*size.height() && i > 0.5*polygonal_spiral::top_left_y(0.5)*size.height()) //левая_нижняя1
                            || (polygonal_spiral::line(r, angle+3*M_PI/2-M_PI/5, polygonal_spiral::pentagon_inner_radius(1), polygonal_spiral_thickness)
                                && i < polygonal_spiral::top_left_y(0.5)*size.height() && i > 0) // левая_верхняя1
                            || (polygonal_spiral::line(r, angle+3*M_PI/2+M_PI/5, polygonal_spiral::pentagon_inner_radius(1), polygonal_spiral_thickness)
                                && i < polygonal_spiral::top_left_y(0.5)*size.height() && i > 0)  //правая_верхняя1
                            || (polygonal_spiral::line(r, angle+M_PI/10, polygonal_spiral::pentagon_inner_radius(1), polygonal_spiral_thickness)
                                && i < (0.5+polygonal_spiral::pentagon_inner_radius(0.5))*size.height() && i > 0.5*polygonal_spiral::top_left_y(0.5)*size.height());  //правая_нижняя1
                    for (int p = 0; p < gauss_beam.polygonal_spiral_.total; p++) {
                        if (p < gauss_beam.polygonal_spiral_.skip) {
                            continue;
                        }
                        squareSpiral2 = squareSpiral2 || (polygonal_spiral::line(r, angle+M_PI/2, polygonal_spiral::pentagon_inner_radius(1)-p*pitch*2, polygonal_spiral_thickness)
                                    && j < (1-polygonal_spiral::bottom_left_x(0.5)-p*pitch/cos(M_PI/10)+p*pitch*tan(M_PI/10))*size.width() && j > (polygonal_spiral::bottom_left_x(0.5)+(p+1)*pitch/cos(M_PI/10)-p*pitch*tan(M_PI/10))*size.width()) //нижняя1
                                || (polygonal_spiral::line(r, angle+M_PI-M_PI/10, polygonal_spiral::pentagon_inner_radius(1)-(p+1)*pitch*2, polygonal_spiral_thickness)
                                    && i < (0.5+polygonal_spiral::pentagon_inner_radius(0.5)-p*pitch)*size.height() && i > (1-polygonal_spiral::top_left_y(0.5)+(p+1)*pitch*sin(M_PI/10)/cos(M_PI/5))*size.height()) // левая_нижняя1
                                || (polygonal_spiral::line(r, angle+3*M_PI/2-M_PI/5, polygonal_spiral::pentagon_inner_radius(1)-(p+1)*pitch*2, polygonal_spiral_thickness)
                                    && i < (1-polygonal_spiral::top_left_y(0.5)+(p+1)*pitch*sin(M_PI/10)/cos(M_PI/5))*size.height() && i > (p+1)*pitch/cos(M_PI/5)*size.height()) // левая_верхняя1
                                || (polygonal_spiral::line(r, angle+3*M_PI/2+M_PI/5, polygonal_spiral::pentagon_inner_radius(1)-(p+1)*pitch*2, polygonal_spiral_thickness)
                                    && i < (1-polygonal_spiral::top_left_y(0.5)+(p+1)*pitch*sin(M_PI/10)/cos(M_PI/5))*size.height() && i > (p+1)*pitch/cos(M_PI/5)*size.height())  //правая_верхняя1
                                || (polygonal_spiral::line(r, angle+M_PI/10, polygonal_spiral::pentagon_inner_radius(1)-(p+1)*pitch*2, polygonal_spiral_thickness)
                                    && i < (0.5+polygonal_spiral::pentagon_inner_radius(0.5)-(p+1)*pitch)*size.height() && i > (1-polygonal_spiral::top_left_y(0.5)+(p+1)*pitch*sin(M_PI/10)/cos(M_PI/5))*size.height());  //правая_нижняя1
                    }
                    break;
                }
                if ((squareSpiral1 && gauss_beam.polygonal_spiral_.skip == 0)|| squareSpiral2) {
                    ref_beam.at(i).push_back(exp(-(pow(x - gauss_beam.shift, 2) + y * y) / (2 * gauss_beam.sigma * gauss_beam.sigma)));
                } else {
                    ref_beam.at(i).push_back(0);
                }
                break;
            case amplitude_type::curve_type :
                ref_beam.at(i).push_back((exp(-(pow(x - gauss_beam.shift, 2) + y * y) / (2 * gauss_beam.sigma * gauss_beam.sigma))));
                break;
            case amplitude_type::spiral_type :
                double lambda = gauss_beam.spiral_.lambda/1e9;
                if (abs(r*r - gauss_beam.spiral_.r0*gauss_beam.spiral_.r0 - ph_size_coefficient*angle*gauss_beam.spiral_.z*gauss_beam.spiral_.l*lambda/M_PI) < gauss_beam.spiral_.d) {
                    ref_beam.at(i).push_back((exp(-(pow(x - gauss_beam.shift, 2) + y * y) / (2 * gauss_beam.sigma * gauss_beam.sigma))));
                } else {
                    ref_beam.at(i).push_back(0);
                }
                break; 
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

bool gauss_beam::polygonal_spiral_gauss_param_preprocessing(QLineEdit* sigma_line, QLineEdit* shift_line, QLineEdit* shift_angle_line, QLineEdit* pitch_line, QLineEdit* total_line, QLineEdit* skip_line, QLineEdit* spiral_thickness_line, QComboBox* polygonal_spiral_type_combo_box, gauss_beam &gauss_beam) {
    if (sigma_line->text().isEmpty() || shift_line->text().isEmpty() || shift_angle_line->text().isEmpty()) {
        return false;
    }
    bool sigma_line_to_string_ok, shift_line_to_string_ok, shift_angle_line_to_string_ok, pitch_line_to_string_ok, total_line_to_string_ok, skip_line_to_string_ok, spiral_thickness_line_to_string_ok;
    double sigma = sigma_line->text().toDouble(&sigma_line_to_string_ok);
    double shift = shift_line->text().toDouble(&shift_line_to_string_ok);
    double shift_angle = shift_angle_line->text().toDouble(&shift_angle_line_to_string_ok);
    double pitch = pitch_line->text().toDouble(&pitch_line_to_string_ok);
    double total = total_line->text().toDouble(&total_line_to_string_ok);
    double skip = skip_line->text().toDouble(&skip_line_to_string_ok);
    double d = spiral_thickness_line->text().toDouble(&spiral_thickness_line_to_string_ok);
    if (shift_line_to_string_ok && sigma_line_to_string_ok && shift_angle_line_to_string_ok && pitch_line_to_string_ok && total_line_to_string_ok && skip_line_to_string_ok && spiral_thickness_line_to_string_ok) {
        class gauss_beam result_gauss(sigma, shift, shift_angle, polygonal_spiral(pitch, total, skip, d, static_cast<enum polygonal_spiral_type>(polygonal_spiral_type_combo_box->currentIndex())));
        gauss_beam = result_gauss;
        return true;
    } else {
        return false;
    }
}

bool gauss_beam::curve_gauss_param_preprocessing(QLineEdit* sigma_line, QLineEdit* shift_line, QLineEdit* shift_angle_line, QLineEdit* a_line, QLineEdit* b_line, QLineEdit* m_line, QLineEdit* n1_line, QLineEdit* n2_line, QLineEdit* n3_line, QLineEdit* curve_thickness_line, QLineEdit* ph_size_line, gauss_beam &gauss_beam) {
    if (sigma_line->text().isEmpty() || shift_line->text().isEmpty() || shift_angle_line->text().isEmpty()) {
        return false;
    }
    class curve curve_;
    if (curve::curve_param_preprocessing(a_line, b_line, m_line, n1_line, n2_line, n3_line, curve_thickness_line, ph_size_line, curve_)) {
        bool sigma_line_to_string_ok, shift_line_to_string_ok, shift_angle_line_to_string_ok;
        double sigma = sigma_line->text().toDouble(&sigma_line_to_string_ok);
        double shift = shift_line->text().toDouble(&shift_line_to_string_ok);
        double shift_angle = shift_angle_line->text().toDouble(&shift_angle_line_to_string_ok);
        if (shift_line_to_string_ok && sigma_line_to_string_ok && shift_angle_line_to_string_ok) {
            class gauss_beam result_gauss(sigma, shift, shift_angle, curve_);
            gauss_beam = result_gauss;
            return true;
        } else {
            return false;
        }
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
    polygonal_spiral_ = obj.polygonal_spiral_;
    curve_ = obj.curve_;
    type = obj.type;
    return *this;
}
