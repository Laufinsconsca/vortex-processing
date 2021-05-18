#include "enums.h"

QString color_scheme_to_string(scheme color) {
    switch (color) {
    case scheme::gray:
        return "gray";
    case scheme::dusk:
        return "dusk";
    case scheme::dawn:
        return "dawn";
    case scheme::fire:
        return "fire";
    case scheme::seashore:
        return "seashore";
    case scheme::kryptonite:
        return "kryptonite";
    case scheme::teals:
        return "teals";
    case scheme::viridis:
        return "viridis";
    case scheme::plasma:
        return "plasma";
    case scheme::rainbow:
        return "rainbow";
    case scheme::rainbow_2:
        return "rainbow_2";
    case scheme::rainbow_3:
        return "rainbow_3";
    case scheme::rainbow_4:
        return "rainbow_4";
    case scheme::rainbow_5:
        return "rainbow_5";
    default:
        return "Unknown color map";
    }
}

QVector<QRgb> color_map(scheme color_scheme) {
    QVector<QRgb> rgb_vector;
    int buf;
    QColor buf_color;
    QString path = ":/cmap/" + color_scheme_to_string(color_scheme) + ".rgb";
    QFile data(path.toStdString().c_str());
    if (data.exists() && data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        if (!in.atEnd()) {
            for (int i = 0; i < 256; i++) {
                in >> buf;
                buf_color.setRed(buf);
                in >> buf;
                buf_color.setGreen(buf);
                in >> buf;
                buf_color.setBlue(buf);
                rgb_vector.append(buf_color.rgb());
            }
        }
        data.close();
    } else {
        QString exception_name = "File " + path + " not found!";
        throw std::runtime_error(exception_name.toStdString());
    }
    return rgb_vector;
}
