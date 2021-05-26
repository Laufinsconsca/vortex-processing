#ifndef ENUMS_H
#define ENUMS_H

#include <QString>
#include <QRgb>
#include <QColor>
#include <QFile>
#include <QTextStream>
#include <stdexcept>

enum scheme {
    gray,
    fire,
    viridis,
    plasma,
    dawn,
    dusk,
    seashore,
    kryptonite,
    teals,
    rainbow,
    rainbow_2,
    rainbow_3,
    rainbow_4,
    rainbow_5,
    green
};

enum direction {
    row,
    column
};


enum in_type_field {
    gauss,
    vortex
};


enum out_field_type {
    amplitude,
    phase,
    intensity,
    oam
};

enum hole_type {
    amplitude_hole,
    phase_hole,
    amplitude_phase_hole,
    none
};

QString color_scheme_to_string(scheme color);
QVector<QRgb> color_map(scheme color_scheme);

#endif // ENUMS_H
