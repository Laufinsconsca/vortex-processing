#ifndef HOLE_H
#define HOLE_H

#include <QLineEdit>
#include <vector>
#include <QtMath>
#include <QImage>
#include <math.h>
#include "enums.h"
#include "log.h"

class hole {
public:
    /** The distance from the center of an image to the center of the hole */
    double r_d;
    /** The hole radius */
    double r_hole;
    /** The angle of the hole */
    double fi;
    hole_type type;
    hole();
    hole(double r_d, double r_hole, double fi, hole_type type);
    void set_hole_type(hole_type hole_type);
    static void create_hole(std::vector<std::vector<double>>& image, const hole& hole, double r);
    static void create_hole(QImage& image, const hole& hole, double r);
    static bool holes_param_preprocessing(QLineEdit* r_d_line, QLineEdit* r_hole_line, QLineEdit* r_fi_line, hole_type hole_type, bool is_hole_type_changed, QLineEdit* sender, hole& old_hole);
    hole& operator=(const hole& obj);
};

#endif // HOLE_H
