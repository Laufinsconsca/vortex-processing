#include "settings.h"
#include "ui_settings.h"

bool Settings::compare(QString arg1, QString arg2) {
    return Ui::scheme_name_map[arg1].first < Ui::scheme_name_map[arg2].first;
}

Settings::Settings(QWidget *parent) : QWidget(parent), ui(new Ui::Settings) {
    ui->setupUi(this);
    QList<QString> strings = QList<QString>(Ui::scheme_name_map.keys());
    std::sort(strings.begin(), strings.end(), compare);
    QStringListModel *model = new QStringListModel();
    model->setStringList(strings);
    ui->in_amplitude_color_scheme_combo_box->setModel(model);
    ui->size_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->in_amplitude_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->out_amplitude_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->in_phase_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->out_phase_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->intensity_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->oam_color_scheme_combo_box->setFont(QFont("Arial", 10, QFont::Normal));
    ui->in_amplitude_color_scheme_combo_box->setModel(model);
    ui->out_amplitude_color_scheme_combo_box->setModel(model);
    ui->in_phase_color_scheme_combo_box->setModel(model);
    ui->out_phase_color_scheme_combo_box->setModel(model);
    ui->intensity_color_scheme_combo_box->setModel(model);
    ui->oam_color_scheme_combo_box->setModel(model);
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

void Settings::choose_scheme_template(const QString &arg1, QLabel* label, void (Settings::*signal)(scheme)) {
    QPixmap pixmap(":/scales/" + Ui::scheme_name_map[arg1].second + "_256.bmp");
    label->setPixmap(pixmap);
    emit (this->*signal)((scheme)Ui::scheme_name_map[arg1].first);
}

void Settings::on_in_amplitude_color_scheme_combo_box_textActivated(const QString &arg1) {
    choose_scheme_template(arg1, ui->in_amplitude_color_scheme_label, &Settings::send_in_amplitude_color_scheme);
}

void Settings::on_in_phase_color_scheme_combo_box_textActivated(const QString &arg1) {
    choose_scheme_template(arg1, ui->in_phase_color_scheme_label, &Settings::send_in_phase_color_scheme);
}

void Settings::on_out_amplitude_color_scheme_combo_box_textActivated(const QString &arg1) {
    choose_scheme_template(arg1, ui->out_amplitude_color_scheme_label, &Settings::send_out_amplitude_color_scheme);
}

void Settings::on_out_phase_color_scheme_combo_box_textActivated(const QString &arg1) {
    choose_scheme_template(arg1, ui->out_phase_color_scheme_label, &Settings::send_out_phase_color_scheme);
}

void Settings::on_intensity_color_scheme_combo_box_textActivated(const QString &arg1) {
    choose_scheme_template(arg1, ui->intensity_color_scheme_label, &Settings::send_intensity_color_scheme);
}

void Settings::on_oam_color_scheme_combo_box_textActivated(const QString &arg1) {
    choose_scheme_template(arg1, ui->oam_color_scheme_label, &Settings::send_oam_color_scheme);
}
