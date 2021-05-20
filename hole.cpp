#include "hole.h"

hole::hole() {

}

/**
 * Constructor
 *
 * @param	r_d   	The distance from the center of an vortex to the center of the hole
 * @param	r_hole	The hole radius
 * @param	fi	  	The angle of the hole
 **************************************************************************************************/

hole::hole(double r_d, double r_hole, double fi, hole_type type) : r_d(r_d), r_hole(r_hole), fi(fi), type(type) {}

/**
 * Create a hole
 *
 * @param   image   A reference to the image vector to be modified
 * @param	hole	The hole
 * @param	r	  	The relative width of an image (by default r = 1)
 **************************************************************************************************/

void hole::create_hole(std::vector<std::vector<double>>& image, const hole& hole, double r) {
    double fi = hole.fi*2*M_PI/360 + M_PI_2;
    double height = image.size();
    double width = image.at(0).size();
    double min_size = height < width ? height : width;
    double t_cos = (width + cos(fi) * hole.r_d * width / r) / 2;
    double t_sin = (height + sin(fi) * hole.r_d * height / r) / 2;
    for (int i = round(t_cos - hole.r_hole * width / (2 * r)); i < round(t_cos + hole.r_hole * width / (2 * r)); i++) {
        for (int j = round(t_sin - hole.r_hole * height / (2 * r)); j < round(t_sin + hole.r_hole * height / (2 * r)); j++) {
            if (pow(i - t_cos, 2) + pow(j - t_sin, 2) < pow(hole.r_hole * min_size / (2 * r), 2)) {
                image.at(i).at(j) = 0;
            }
        }
    }
}

void hole::set_hole_type(hole_type hole_type) {
    type = hole_type;
}


/**
 * Create a hole
 *
 * @param   image   A reference to the QImage to be modified
 * @param	hole	The hole
 * @param	r	  	The relative width of an image (by default r = 1)
 **************************************************************************************************/

void hole::create_hole(QImage& image, const hole& hole, double r) {
    double fi = hole.fi*2*M_PI/360 + M_PI_2;
    double height = image.size().height();
    double width = image.size().width();
    double min_size = height < width ? height : width;
    double t_sin = (height + sin(fi) * hole.r_d * height / r) / 2;
    double t_cos = (width + cos(fi) * hole.r_d * width / r) / 2;
    for (int i = round(t_cos - hole.r_hole * width / (2 * r)); i < round(t_cos + hole.r_hole * width / (2 * r)); i++) {
        for (int j = round(t_sin - hole.r_hole * height / (2 * r)); j < round(t_sin + hole.r_hole * height / (2 * r)); j++) {
            if (pow(i - t_cos, 2) + pow(j - t_sin, 2) < pow(hole.r_hole * min_size / (2 * r), 2)) {
                image.setPixel(j, i, 0);
            }
        }
    }
}

bool hole::holes_param_preprocessing(QLineEdit* r_d_line, QLineEdit* r_hole_line, QLineEdit* r_fi_line, hole_type hole_type, bool is_hole_type_changed, QLineEdit* sender, hole& old_hole) {

    FUNCTION_LOG

    if (hole_type == hole_type::none) { // если нет отверстия, то false
        return true;
    }
    if (r_d_line->text().isEmpty() // если строки пустые, то false
            || r_hole_line->text().isEmpty()
            || r_fi_line->text().isEmpty()) {
        return false;
    }
    bool r_d_line_to_string_ok, r_hole_line_to_string_ok, r_fi_line_to_string_ok;
    double r_d = r_d_line->text().toDouble(&r_d_line_to_string_ok);
    double r_hole = r_hole_line->text().toDouble(&r_hole_line_to_string_ok);
    if (r_d_line_to_string_ok
            && r_hole_line_to_string_ok) {
        if (r_d > 1 || r_hole > 1) {
            return false;
        }
        if (r_d + r_hole > 1) {
            if (sender == r_hole_line) {
                r_hole = 1 - r_d;
                r_hole_line->setText(QString::number(r_hole));
            } else if (sender == r_d_line) {
                r_d = 1 - r_hole;
                r_d_line->setText(QString::number(r_d));
            }
            return false;
        }
        double r_fi = r_fi_line->text().toDouble(&r_fi_line_to_string_ok);
        if (r_fi_line_to_string_ok
                && (is_hole_type_changed
                    || (old_hole.r_d != r_d)
                    || (old_hole.r_hole != r_hole)
                    || (old_hole.fi != r_fi))) {
            if (r_hole == 0) { // если радиус отверстия 0, то false
                return false;
            }
            old_hole = hole(r_d, r_hole, r_fi, hole_type);
            return true;
        } else {
            return false;
        }
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

hole& hole::operator=(const hole& obj) {
    if (this == &obj) {
        return *this;
    }
    r_d = obj.r_d;
    r_hole = obj.r_hole;
    fi = obj.fi;
    return *this;
}
