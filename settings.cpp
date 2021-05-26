#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) : QWidget(parent), ui(new Ui::Settings) {
    ui->setupUi(this);
    ui->size_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->in_amplitude_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->out_amplitude_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->in_phase_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->out_phase_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->intensity_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->oam_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
}

Settings::~Settings() {
    delete ui;
}

void Settings::on_size_combo_box_textActivated(const QString &arg1) {
    QSize size;
    QStringList list = arg1.split("×");
    size.setHeight(list.at(0).toInt());
    size.setWidth(list.at(1).toInt());
    emit send_size(size);
}

void Settings::on_in_amplitude_color_scheme_combo_box_textActivated(const QString &arg1) {
    QPixmap pixmap(":/scales/" + string_to_scheme_name(arg1) + "_256.bmp");
    ui->in_amplitude_color_scheme_label->setPixmap(pixmap);
    emit send_in_amplitude_color_scheme(string_to_scheme(arg1));
}

void Settings::on_in_phase_color_scheme_combo_box_textActivated(const QString &arg1) {
    QPixmap pixmap(":/scales/" + string_to_scheme_name(arg1) + "_256.bmp");
    ui->in_phase_color_scheme_label->setPixmap(pixmap);
    emit send_in_phase_color_scheme(string_to_scheme(arg1));
}

void Settings::on_out_amplitude_color_scheme_combo_box_textActivated(const QString &arg1) {
    QPixmap pixmap(":/scales/" + string_to_scheme_name(arg1) + "_256.bmp");
    ui->out_amplitude_color_scheme_label->setPixmap(pixmap);
    emit send_out_amplitude_color_scheme(string_to_scheme(arg1));
}

void Settings::on_out_phase_color_scheme_combo_box_textActivated(const QString &arg1) {
    QPixmap pixmap(":/scales/" + string_to_scheme_name(arg1) + "_256.bmp");
    ui->out_phase_color_scheme_label->setPixmap(pixmap);
    emit send_out_phase_color_scheme(string_to_scheme(arg1));
}

void Settings::on_intensity_color_scheme_combo_box_textActivated(const QString &arg1) {
    QPixmap pixmap(":/scales/" + string_to_scheme_name(arg1) + "_256.bmp");
    ui->intensity_color_scheme_label->setPixmap(pixmap);
    emit send_intensity_color_scheme(string_to_scheme(arg1));
}

void Settings::on_oam_color_scheme_combo_box_textActivated(const QString &arg1) {
    QPixmap pixmap(":/scales/" + string_to_scheme_name(arg1) + "_256.bmp");
    ui->oam_color_scheme_label->setPixmap(pixmap);
    emit send_oam_color_scheme(string_to_scheme(arg1));
}

scheme Settings::string_to_scheme(const QString& arg) {
    if (arg == "серая") {
        return scheme::gray;
    } else if (arg == "dusk") {
        return scheme::dusk;
    } else if (arg == "dawn") {
        return scheme::dawn;
    } else if (arg == "огненная") {
        return scheme::fire;
    } else if (arg == "seashore") {
        return scheme::seashore;
    } else if (arg == "kryptonite") {
        return scheme::kryptonite;
    } else if (arg == "teals") {
        return scheme::teals;
    } else if (arg == "радужная 1") {
        return scheme::rainbow;
    } else if (arg == "радужная 2") {
        return scheme::rainbow_2;
    } else if (arg == "радужная 3") {
        return scheme::rainbow_3;
    } else if (arg == "радужная 4") {
        return scheme::rainbow_4;
    } else if (arg == "радужная 5") {
        return scheme::rainbow_5;
    } else if (arg == "viridis") {
        return scheme::viridis;
    } else if (arg == "plasma") {
        return scheme::plasma;
    } else if (arg == "зеленая") {
        return scheme::green;
    } else {
        throw new std::runtime_error("Неизвестная цветовая схема");
    }
}

QString Settings::string_to_scheme_name(const QString& arg) {
    if (arg == "серая") {
        return "gray";
    } else if (arg == "dusk") {
        return "dusk";
    } else if (arg == "dawn") {
        return "dawn";
    } else if (arg == "огненная") {
        return "fire";
    } else if (arg == "seashore") {
        return "seashore";
    } else if (arg == "kryptonite") {
        return "kryptonite";
    } else if (arg == "teals") {
        return "teals";
    } else if (arg == "радужная 1") {
        return "rainbow";
    } else if (arg == "радужная 2") {
        return "rainbow_2";
    } else if (arg == "радужная 3") {
        return "rainbow_3";
    } else if (arg == "радужная 4") {
        return "rainbow_2";
    } else if (arg == "радужная 5") {
        return "rainbow_5";
    } else if (arg == "viridis") {
        return "viridis";
    } else if (arg == "plasma") {
        return "plasma";
    } else if (arg == "зеленая") {
        return "green";
    } else {
        throw new std::runtime_error("Неизвестная цветовая схема");
    }
}
