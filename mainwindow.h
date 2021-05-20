#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QValidator>
#include <QErrorMessage>
#include <QLabel>
#include "custom_validators.h"
#include "complex_amplitude.h"
#include "settings.h"
#include "log.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void recieve_size(QSize& size);

    void recieve_in_amplitude_color_scheme(scheme color_scheme);

    void recieve_in_phase_color_scheme(scheme color_scheme);

    void recieve_out_amplitude_color_scheme(scheme color_scheme);

    void recieve_out_phase_color_scheme(scheme color_scheme);

    void recieve_intensity_color_scheme(scheme color_scheme);

    void recieve_oam_color_scheme(scheme color_scheme);

private slots:
    void on_fft_clicked();

    void on_find_oam_clicked();

    void on_load_amplitude_triggered();

    void on_load_phase_triggered();

    void display_spp();

    void display_gauss_beam();

    void display_both();

    void on_fft_expansion_line_editingFinished();

    void on_save_intensity_triggered();

    void on_save_amplitude_triggered();

    void on_save_phase_triggered();

    void on_save_oam_triggered();

    void on_comboBox_textActivated(const QString &arg1);

    void on_settings_triggered();

private:
    Ui::MainWindow *ui;
    QSize image_to_save_size{256, 256};
    scheme in_amplitude_color_scheme = scheme::fire;
    scheme in_phase_color_scheme = scheme::rainbow;
    scheme out_amplitude_color_scheme = scheme::fire;
    scheme out_phase_color_scheme = scheme::rainbow;
    scheme intensity_color_scheme = scheme::fire;
    scheme oam_color_scheme = scheme::fire;
    int fft_expansion;
    double m;
    double fi;
    double sigma;
    double shift;
    hole_type hole_type = hole_type::none;
    bool is_hole_type_changed;
    class vortex vortex_ {
        0,0
    };
    gauss_beam gauss_beam_{0,0};
    hole hole_{0,0,0,hole_type::none};
    bool is_amplitude_from_file = false;
    bool is_phase_from_file = false;
    //complex_amplitude complex_amplitude_;
    QImage amplitude_from_file;
    QImage phase_from_file;
    QImage out_amplitude_cur;
    QImage out_phase_cur;
    QImage intensity_cur;
    QImage oam_density_cur;
    QString filters{"BMP (*.bmp);;PNG (*.png);;JPG (*.jpg);;All files (*.*)"};
    QString defaultFilter{"BMP (*.bmp)"};
    void save(out_field_type type, scheme color_scheme, QString description);
    void read_complex_amplitude(complex_amplitude& complex_amplitude_, QSize& size);
    Settings* settings;

    void auxiliary_function_to_process_changing_color_scheme(QLabel* image_label, QLabel* scale_label, QImage image, scheme color_scheme_to_choose, scheme& prev_color_scheme);

    //bool spp_param_preprocessing();

    //bool gauss_param_preprocessing();

    //bool holes_param_preprocessing();

};
#endif // MAINWINDOW_H
