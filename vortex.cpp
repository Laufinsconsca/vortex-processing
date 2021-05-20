#include "vortex.h"

/**
 * Constructor.
 *
 * @param	f	  	The dependency topological charge from distance from the center of an vortex
 * @param	pow_fi	The power of fi.
 * @param	r_d   	The distance from the center of an vortex to the center of the hole.
 * @param	r_hole	The hole radius.
 * @param	fi	  	The angle of the hole.
 **************************************************************************************************/

vortex::vortex(std::function<double(double)> f, double pow_fi) : f_tp(f), pow_fi(pow_fi) {}

/**
 * Constructor.
 *
 * @param	tp	  	The constant topological charge value (if there is no dependence on the distance).
 * @param	pow_fi	The power of fi.
 * @param	r_d   	The distance from the center of an vortex to the center of the hole.
 * @param	r_hole	The hole radius.
 * @param	fi	  	The angle of the hole.
 **************************************************************************************************/

vortex::vortex(double tp, double pow_fi) : pow_fi(pow_fi) {
    f_tp = [=](double r) {
        return tp;
    };
}

/**
 * Get value of the topological charge for given distance.
 *
 * @param	distance Distance from the center of the image.
 *
 * @returns	The topological charge at a given distance from the center.
 **************************************************************************************************/

double vortex::tp(double distance) const {
    return f_tp(distance);
}

std::vector<std::vector<double>>& vortex::vortex_to_vector(const vortex& vortex, std::vector<std::vector<double>>& spp, const QSize& size) {
    double hx, hy, x, y;
    hx = 2. / size.width();
    hy = 2. / size.height();
    spp.reserve(size.height());
    for (int i = 0; i < size.height(); i++) {
        y = i * hy - 1;
        spp.push_back(std::vector<double>());
        spp.at(i).reserve(size.width());
        for (int j = 0; j < size.width(); j++) {
            x = j * hx - 1;
            double angle = std::atan2(-y, x);
            double tp = vortex.tp(sqrt(x * x + y * y));
            spp.at(i).push_back(tp * (angle > 0 ? pow(angle, vortex.pow_fi) : pow(angle + 2 * M_PI, vortex.pow_fi)));
        }
    }
    return spp;
}

std::vector<std::vector<double>>& vortex::vortex_to_vector(const vortex &vortex, std::vector<std::vector<double> > &spp, const QSize& size, const hole &hole) {
    vortex_to_vector(vortex, spp, size);
    hole::create_hole(spp, hole, 1);
    return spp;
}

bool vortex::spp_param_preprocessing(QLineEdit* pow_fi_line, QLineEdit* fi_line, class vortex& vortex) {
    if (pow_fi_line->text().isEmpty() || fi_line->text().isEmpty()) {
        return false;
    }
    bool m_line_to_string_ok, fi_line_to_string_ok;
    double pow_fi = pow_fi_line->text().toDouble(&m_line_to_string_ok);
    double fi = fi_line->text().toDouble(&fi_line_to_string_ok);
    if (m_line_to_string_ok && fi_line_to_string_ok) {
        class vortex a_vortex(pow_fi, fi);
        vortex = a_vortex;
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

class vortex& vortex::vortex::operator=(const vortex &obj) {
    if (this == &obj) {
        return *this;
    }
    f_tp = obj.f_tp;
    pow_fi = obj.pow_fi;
    return *this;
}
