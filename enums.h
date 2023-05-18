#ifndef ENUMS_H
#define ENUMS_H

#include <QVector>
#include <QString>
#include <QRgb>
#include <QColor>
#include <QFile>
#include <QTextStream>
#include <stdexcept>

enum scheme {
    gray,
    fire,
    green,
    rainbow_1,
    rainbow_2,
    rainbow_3,
    rainbow_4,
    rainbow_5,
    dawn,
    dusk,
    plasma,
    viridis,
    seashore,
    kryptonite,
    teals,
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

enum amplitude_type {
    gauss_type,
    spiral_type,
    polygonal_spiral_type,
    curve_type
};

enum spiral_type {
    archimedean,
    logarithmic
};

enum polygonal_spiral_type {
    triangular,
    square,
    pentagonal
};

QString color_scheme_to_string(scheme color);
QVector<QRgb> load_color_map(scheme color_scheme);

#endif // ENUMS_H
