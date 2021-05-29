#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QLabel>
#include <QStringListModel>
#include "enums.h"

namespace Ui {
static QMap<QString, std::pair<int,QString>> scheme_name_map{
    std::pair<QString,std::pair<int,QString>>("серая",std::pair<int,QString>(scheme::gray, "gray")),
    std::pair<QString,std::pair<int,QString>>("огненная",std::pair<int,QString>(scheme::fire, "fire")),
    std::pair<QString,std::pair<int,QString>>("зеленая",std::pair<int,QString>(scheme::green, "green")),
    std::pair<QString,std::pair<int,QString>>("радужная",std::pair<int,QString>(scheme::rainbow_1, "rainbow_1")),
    std::pair<QString,std::pair<int,QString>>("радужная 2",std::pair<int,QString>(scheme::rainbow_2, "rainbow_2")),
    std::pair<QString,std::pair<int,QString>>("радужная 3",std::pair<int,QString>(scheme::rainbow_3, "rainbow_3")),
    std::pair<QString,std::pair<int,QString>>("радужная 4",std::pair<int,QString>(scheme::rainbow_4, "rainbow_4")),
    std::pair<QString,std::pair<int,QString>>("радужная 5",std::pair<int,QString>(scheme::rainbow_5, "rainbow_5")),
    std::pair<QString,std::pair<int,QString>>("dawn",std::pair<int,QString>(scheme::dawn, "dawn")),
    std::pair<QString,std::pair<int,QString>>("dusk",std::pair<int,QString>(scheme::dusk, "dusk")),
    std::pair<QString,std::pair<int,QString>>("plasma",std::pair<int,QString>(scheme::plasma, "plasma")),
    std::pair<QString,std::pair<int,QString>>("viridis",std::pair<int,QString>(scheme::viridis, "viridis")),
    std::pair<QString,std::pair<int,QString>>("seashore",std::pair<int,QString>(scheme::seashore, "seashore")),
    std::pair<QString,std::pair<int,QString>>("kryptonite",std::pair<int,QString>(scheme::kryptonite, "kryptonite")),
    std::pair<QString,std::pair<int,QString>>("teals",std::pair<int,QString>(scheme::teals, "teals"))
};
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
    static bool compare(QString arg1, QString arg2);
    void choose_scheme_template(const QString &arg1, QLabel* label, void (Settings::*signal)(scheme));

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
