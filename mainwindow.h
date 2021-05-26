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

    void on_save_in_amplitude_triggered();

    void on_save_in_phase_triggered();

    void on_save_intensity_triggered();

    void on_save_out_amplitude_triggered();

    void on_save_out_phase_triggered();

    void on_save_oam_triggered();

    void on_comboBox_textActivated(const QString &arg1);

    void on_settings_triggered();

    void on_save_all_out_distributions_triggered();

private:
    Ui::MainWindow *ui;
    QSize image_to_save_size{256, 256};
    scheme in_amplitude_color_scheme = scheme::fire;
    scheme in_phase_color_scheme = scheme::rainbow_5;
    scheme out_amplitude_color_scheme = scheme::fire;
    scheme out_phase_color_scheme = scheme::rainbow_5;
    scheme intensity_color_scheme = scheme::fire;
    scheme oam_color_scheme = scheme::fire;
    int fft_expansion;
    double m;
    double fi;
    double sigma;
    double shift;
    hole_type hole_type = hole_type::none;
    bool is_hole_type_changed;
    class vortex vortex_;
    gauss_beam gauss_beam_;
    hole hole_;
    bool is_amplitude_from_file = false;
    bool is_phase_from_file = false;
    QImage amplitude_from_file;
    QImage phase_from_file;
    QImage out_amplitude_cur;
    QImage out_phase_cur;
    QImage intensity_cur;
    QImage oam_density_cur;
    QString filters{"BMP (*.bmp);;PNG (*.png);;JPG (*.jpg);;All files (*.*)"};
    QString defaultFilter{"BMP (*.bmp)"};
    void save(QString filename, QString format, out_field_type type, scheme color_scheme, bool out_field);
    void save(out_field_type type, scheme color_scheme, QString description, bool out_field);
    void read_complex_amplitude(complex_amplitude& complex_amplitude_, QSize& size);
    Settings* settings;
    QColor background_out_the_circle_in_field_color = QColor(240,240,240);
    QColor background_out_the_circle_in_field_color_to_save = QColor(255,255,255);
    void auxiliary_function_to_process_changing_color_scheme(QLabel* image_label, QLabel* scale_label, QImage image, scheme color_scheme_to_choose, scheme& prev_color_scheme);
};
#endif // MAINWINDOW_H
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
