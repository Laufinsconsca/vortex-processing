#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "enums.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget {
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

signals:
    void send_size(QSize& size);
    void send_in_amplitude_color_scheme(scheme color_scheme);
    void send_in_phase_color_scheme(scheme color_scheme);
    void send_out_amplitude_color_scheme(scheme color_scheme);
    void send_out_phase_color_scheme(scheme color_scheme);
    void send_intensity_color_scheme(scheme color_scheme);
    void send_oam_color_scheme(scheme color_scheme);
private:
    Ui::Settings *ui;
    scheme string_to_scheme(const QString& arg);
    QString string_to_scheme_name(const QString& arg);

private slots:
    void on_size_combo_box_textActivated(const QString &arg1);
    void on_in_amplitude_color_scheme_combo_box_textActivated(const QString &arg1);
    void on_in_phase_color_scheme_combo_box_textActivated(const QString &arg1);
    void on_out_amplitude_color_scheme_combo_box_textActivated(const QString &arg1);
    void on_out_phase_color_scheme_combo_box_textActivated(const QString &arg1);
    void on_intensity_color_scheme_combo_box_textActivated(const QString &arg1);
    void on_oam_color_scheme_combo_box_textActivated(const QString &arg1);
};

#endif // SETTINGS_H
