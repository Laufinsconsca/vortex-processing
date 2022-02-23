#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    settings = new Settings();
    settings->setWindowModality(Qt::ApplicationModal);
    connect(settings, &Settings::send_size, this, &MainWindow::receive_size);
    connect(settings, &Settings::send_in_amplitude_color_scheme, this, &MainWindow::receive_in_amplitude_color_scheme);
    connect(settings, &Settings::send_in_phase_color_scheme, this, &MainWindow::receive_in_phase_color_scheme);
    connect(settings, &Settings::send_out_amplitude_color_scheme, this, &MainWindow::recieve_out_amplitude_color_scheme);
    connect(settings, &Settings::send_out_phase_color_scheme, this, &MainWindow::receive_out_phase_color_scheme);
    connect(settings, &Settings::send_intensity_color_scheme, this, &MainWindow::receive_intensity_color_scheme);
    connect(settings, &Settings::send_oam_color_scheme, this, &MainWindow::receive_oam_color_scheme);
    //connect(settings, &Settings::send_spiral_color_scheme, this, &MainWindow::receive_spiral_color_scheme);
    settings->initialize_color_maps();
    connect(ui->m_line, &QLineEdit::editingFinished, this, &MainWindow::display_spp);
    connect(ui->fi_line, &QLineEdit::editingFinished, this, &MainWindow::display_spp);
    connect(ui->r_d_line, &QLineEdit::editingFinished, this, &MainWindow::display_spp_and_gauss_beam);
    connect(ui->r_hole_line, &QLineEdit::editingFinished, this, &MainWindow::display_spp_and_gauss_beam);
    connect(ui->r_fi_line, &QLineEdit::editingFinished, this, &MainWindow::display_spp_and_gauss_beam);
    connect(ui->sigma_line, &QLineEdit::editingFinished, this, &MainWindow::display_gauss_beam);
    connect(ui->shift_line, &QLineEdit::editingFinished, this, &MainWindow::display_gauss_beam);
    connect(ui->shift_angle_line, &QLineEdit::editingFinished, this, &MainWindow::display_gauss_beam);
    connect(ui->spiral_z_line, &QLineEdit::editingFinished, this, &MainWindow::display_spiral);
    connect(ui->spiral_wavelength_line, &QLineEdit::editingFinished, this, &MainWindow::display_spiral);
    connect(ui->l_line, &QLineEdit::editingFinished, this, &MainWindow::display_spiral);
    connect(ui->r0_line, &QLineEdit::editingFinished, this, &MainWindow::display_spiral);
    connect(ui->spiral_thickness_line, &QLineEdit::editingFinished, this, &MainWindow::display_spiral);
    ui->m_line->setValidator(new QDoubleValidator(-100, 100, 3, this));
    ui->fi_line->setValidator(new QDoubleValidator(-10, 10, 3, this));
    ui->r_fi_line->setValidator(new QIntValidator(0, 360, this));
    ui->sigma_line->setValidator(new QDoubleValidator(0, 5, 3, this));
    ui->shift_line->setValidator(new QDoubleValidator(0, 1, 3, this));
    ui->shift_angle_line->setValidator(new QIntValidator(0, 360, this));
    ui->r_d_line->setValidator(new QDoubleValidator(0, 1, 3, this));
    ui->r_hole_line->setValidator(new QDoubleValidator(0, 1, 6, this));
    ui->z_line->setValidator(new QDoubleValidator(-100, 100, 3, this));
    ui->wavelength_line->setValidator(new QDoubleValidator(0, 1000, 3, this));
    ui->z_line->setValidator(new QDoubleValidator(0, 100000, 3, this));
    ui->spiral_wavelength_line->setValidator(new QDoubleValidator(0, 100000, 3, this));
    ui->l_line->setValidator(new QDoubleValidator(-100, 100, 3, this));
    ui->r0_line->setValidator(new QDoubleValidator(0, 1, 3, this));
    ui->spiral_thickness_line->setValidator(new QDoubleValidator(0, 0.5, 3, this));
    QFont font1 = ui->total_oam_label->font();
    font1.setPointSize(12);
    ui->total_oam_label->setFont(font1);
    QFont font2 = ui->phase_type_label->font();
    font2.setPointSize(10);
    ui->phase_type_label->setFont(font2);
    QIntPowerOf2Validator* validator = new QIntPowerOf2Validator(this);
    connect(validator,  SIGNAL(setError(QString,int)),
            ui->statusbar, SLOT(showMessage(QString,int)));
    ui->fft_expansion_line->setValidator(validator);
    on_fft_expansion_line_editingFinished();
    is_init = false;
    ui->z_line->setFocusPolicy(Qt::StrongFocus);
    ui->z_line->setFocus();
    /* display the initial phase and gauss distributions*/
    display_spp_and_gauss_beam();
    //display_spiral();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_fft_clicked() {
    FUNCTION_LOG

    complex_amplitude complex_amplitude_ = complex_amplitude(in_amplitude, in_phase, hole_);
    if (ui->z_line->text().toDouble() == 0) {
        complex_amplitude_.FFT2D(fft_expansion);
    } else {
        complex_amplitude_.FresnelT(ui->ph_size_line->text().toDouble(),ui->z_line->text().toDouble(), ui->wavelength_line->text().toDouble(), fft_expansion);
    }
    intensity_cur = complex_amplitude_.get_qimage(out_field_type::intensity, scheme::gray);
    out_amplitude_cur = complex_amplitude_.get_qimage(out_field_type::amplitude, scheme::gray);
    out_phase_cur = complex_amplitude_.get_qimage(out_field_type::phase, scheme::gray);
    QImage out_amplitude_to_show = out_amplitude_cur.copy();
    QImage out_phase_to_show = out_phase_cur.copy();
    QImage intensity_to_show = intensity_cur.copy();
    out_amplitude_to_show.setColorTable(load_color_map(out_amplitude_color_scheme));
    out_phase_to_show.setColorTable(load_color_map(out_phase_color_scheme));
    intensity_to_show.setColorTable(load_color_map(intensity_color_scheme));
    ui->amplitude_image->setPixmap(QPixmap::fromImage(out_amplitude_to_show));
    ui->phase_image->setPixmap(QPixmap::fromImage(out_phase_to_show));
    ui->intensity_image->setPixmap(QPixmap::fromImage(intensity_to_show));
}

void MainWindow::on_find_oam_clicked() {
    complex_amplitude complex_amplitude_ = complex_amplitude(in_amplitude, in_phase, hole_);
    if (ui->z_line->text().toDouble() == 0) {
        complex_amplitude_.FFT2D(fft_expansion);
    } else {
        complex_amplitude_.FresnelT(ui->ph_size_line->text().toDouble(),ui->z_line->text().toDouble(), ui->wavelength_line->text().toDouble(), fft_expansion);
    }
    QVector<double> total_oam;
    oam_density_cur = complex_amplitude_.get_oam_qimage(total_oam, scheme::gray);
    QImage oam_density_to_show = oam_density_cur.copy();
    oam_density_to_show.setColorTable(load_color_map(oam_color_scheme));
    ui->oam_density_image->setPixmap(QPixmap::fromImage(oam_density_to_show));
    ui->total_oam_label->setText("Значение общего ОУМ: " + QString(std::to_string(total_oam.at(0)).c_str()));
    ui->min_oam->setText(total_oam.at(1) == 0 ? QString("0") : QString::number(total_oam.at(1), 'e'));
    ui->max_oam->setText(total_oam.at(2) == 0 ? QString("0") : QString::number(total_oam.at(2), 'e'));
}

void MainWindow::on_load_amplitude_triggered() {
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Загрузить амплитуду"),
                       "L:/",
                       tr("Изображения (*.png *.bmp *.jpg)"));
    if (!filename.isEmpty()) {
        QPixmap pixmap(filename);
        in_amplitude= pixmap.toImage();
        ui->gauss_image->setPixmap(pixmap);
        QPixmap scale_pixmap(":/scales/" + color_scheme_to_string(scheme::gray) + "_192.bmp");
        ui->in_amplitude_color_scheme_label->setPixmap(scale_pixmap);
    }
}

void MainWindow::on_load_phase_triggered() {
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Загрузить фазу"),
                       "L:/",
                       tr("Изображения (*.png *.bmp *.jpg)"));
    if (!filename.isEmpty()) {
        QPixmap pixmap(filename);
        in_phase = pixmap.toImage();
        ui->spp_image->setPixmap(pixmap);
        QPixmap scale_pixmap(":/scales/" + color_scheme_to_string(scheme::gray) + "_192.bmp");
        ui->in_phase_color_scheme_label->setPixmap(scale_pixmap);
    }
}

void MainWindow::display_spp() {
    if (is_init) {
        return;
    }
    if (!(hole::holes_param_preprocessing(ui->r_d_line, ui-> r_hole_line, ui->r_fi_line, hole_type, is_hole_type_changed, qobject_cast<QLineEdit*>(sender()), hole_)
            && vortex::spp_param_preprocessing(ui->m_line, ui->fi_line, vortex_))) {
        return;
    }
    class gauss_beam gauss_beam(1, 0, 0); // gauss parameters don't matter
    complex_amplitude a_vortex(gauss_beam, vortex_, size, hole_);
    in_phase = a_vortex.get_qimage(out_field_type::phase, scheme::gray);
    QImage temp = a_vortex.get_qimage(out_field_type::phase, in_phase_color_scheme).copy().convertToFormat(QImage::Format_RGB32);
    complex_amplitude::set_color_out_of_the_circle(temp, background_out_the_circle_in_field_color);
    ui->spp_image->setPixmap(QPixmap::fromImage(temp));
}

void MainWindow::display_gauss_beam() {
    if (is_init) {
        return;
    }
    bool b1 = gauss_beam::gauss_param_preprocessing(ui->sigma_line, ui->shift_line, ui->shift_angle_line, gauss_beam_);
    bool b2 = hole::holes_param_preprocessing(ui->r_d_line, ui-> r_hole_line, ui->r_fi_line, hole_type, is_hole_type_changed, qobject_cast<QLineEdit*>(sender()), hole_);
    if (!(b1 && b2)) {
        return;
    }
    class vortex vortex(1, 1); // vortex parameters don't matter
    complex_amplitude a_vortex(gauss_beam_, vortex, size, hole_);
    in_amplitude = a_vortex.get_qimage(out_field_type::amplitude, scheme::gray);
    QImage temp = a_vortex.get_qimage(out_field_type::amplitude, in_amplitude_color_scheme).copy().convertToFormat(QImage::Format_RGB32);
    complex_amplitude::set_color_out_of_the_circle(temp, background_out_the_circle_in_field_color);
    ui->gauss_image->setPixmap(QPixmap::fromImage(temp));
}

void MainWindow::display_spiral() {
    if (is_init) {
        return;
    }
    if (!(hole::holes_param_preprocessing(ui->r_d_line, ui-> r_hole_line, ui->r_fi_line, hole_type, is_hole_type_changed, qobject_cast<QLineEdit*>(sender()), hole_)
          && gauss_beam::spiral_gauss_param_preprocessing(ui->sigma_line, ui->shift_line, ui->shift_angle_line, ui->spiral_z_line, ui->spiral_wavelength_line, ui->l_line, ui->r0_line, ui->spiral_thickness_line, ui->ph_size_line, gauss_beam_))) {
        return;
    }
    // вырезаем из гаусса спираль
    class vortex vortex(1, 1); // vortex parameters don't matter
    complex_amplitude a_vortex(gauss_beam_, vortex, size, hole_);
    in_amplitude = a_vortex.get_qimage(out_field_type::amplitude, scheme::gray);
    QImage temp = a_vortex.get_qimage(out_field_type::amplitude, in_amplitude_color_scheme).copy().convertToFormat(QImage::Format_RGB32);
    complex_amplitude::set_color_out_of_the_circle(temp, background_out_the_circle_in_field_color);
    ui->gauss_image->setPixmap(QPixmap::fromImage(temp));
}

void MainWindow::display_spp_and_gauss_beam() {
    if (is_init) {
        return;
    }
    bool b1 = gauss_beam::gauss_param_preprocessing(ui->sigma_line, ui->shift_line, ui->shift_angle_line, gauss_beam_);
    bool b2 = hole::holes_param_preprocessing(ui->r_d_line, ui-> r_hole_line, ui->r_fi_line, hole_type, is_hole_type_changed, qobject_cast<QLineEdit*>(sender()), hole_);
    bool b3 = vortex::spp_param_preprocessing(ui->m_line, ui->fi_line, vortex_);
    if (!(b1 && b2 && b3)) {
        return;
    }
    complex_amplitude an_amplitude(gauss_beam_, vortex_, size, hole_);

    in_amplitude = an_amplitude.get_qimage(out_field_type::amplitude, scheme::gray);
    QImage temp = an_amplitude.get_qimage(out_field_type::amplitude, in_amplitude_color_scheme).copy().convertToFormat(QImage::Format_RGB32);
    complex_amplitude::set_color_out_of_the_circle(temp, background_out_the_circle_in_field_color);
    ui->gauss_image->setPixmap(QPixmap::fromImage(temp));

    in_phase = an_amplitude.get_qimage(out_field_type::phase, scheme::gray);
    temp = an_amplitude.get_qimage(out_field_type::phase, in_phase_color_scheme).copy().convertToFormat(QImage::Format_RGB32);
    complex_amplitude::set_color_out_of_the_circle(temp, background_out_the_circle_in_field_color);
    ui->spp_image->setPixmap(QPixmap::fromImage(temp));
}

void MainWindow::on_fft_expansion_line_editingFinished() {
    if (ui->fft_expansion_line->text().isEmpty()) {
        fft_expansion = -1;
        return;
    }
    fft_expansion = ui->fft_expansion_line->text().toInt();
    if (!complex_amplitude::is_power_of_2(fft_expansion)) {
        ui->fft_expansion_line->setText("");
        fft_expansion = 0;
    }
}

void MainWindow::save(QString filename, QString format, out_field_type type, scheme color_scheme, bool out_field) {

    FUNCTION_LOG

    if (!filename.isEmpty() && !format.isEmpty()) {
        complex_amplitude complex_amplitude_ = complex_amplitude(in_amplitude, in_phase, hole_);
        QImage image;
        if (out_field) {
            complex_amplitude_.FFT2D(fft_expansion);
        }
        if (type == out_field_type::oam) {
            QVector<double> total_oam;
            image = complex_amplitude_.get_oam_qimage(total_oam, color_scheme);
            QFile total_oam_file(filename + "_oam.txt");
            QFile min_oam(filename + "_min.txt");
            QFile max_oam(filename + "_max.txt");
            if (total_oam_file.open(QIODevice::WriteOnly)) {
                QTextStream out(&total_oam_file);
                out << total_oam.at(0);
            }
            if (min_oam.open(QIODevice::WriteOnly)) {
                QTextStream out(&min_oam);
                out << total_oam.at(1);
            }
            if (max_oam.open(QIODevice::WriteOnly)) {
                QTextStream out(&max_oam);
                out << total_oam.at(2);
            }
            total_oam_file.close();
            min_oam.close();
            max_oam.close();
            total_oam.clear();
        } else {
            image = complex_amplitude_.get_qimage(type, color_scheme);
            if (!out_field) {
                image = image.convertToFormat(QImage::Format_RGB32);
                complex_amplitude::set_color_out_of_the_circle(image, background_out_the_circle_in_field_color_to_save);
            }
        }
        image.save(filename + "." + format, format.toStdString().c_str());
    }
}

void MainWindow::save(out_field_type type, scheme color_scheme, QString description, bool out_field) {
    QString filename = QFileDialog::getSaveFileName(this,
                       description,
                       QDir::currentPath(),
                       filters,
                       &defaultFilter);
    QStringList pieces = filename.split(".");
    save(filename.left(filename.lastIndexOf('.')), pieces.value(pieces.length() - 1), type, color_scheme, out_field);
}

void MainWindow::on_save_in_amplitude_triggered() {
    save(out_field_type::amplitude, in_amplitude_color_scheme, "Сохранить входную амплитуду", false);
}

void MainWindow::on_save_in_phase_triggered() {
    save(out_field_type::phase, in_phase_color_scheme, "Сохранить входную фазу", false);
}

void MainWindow::on_save_intensity_triggered() {
    save(out_field_type::intensity, intensity_color_scheme, "Сохранить интенсивность", true);
}

void MainWindow::on_save_out_amplitude_triggered() {
    save(out_field_type::amplitude, out_amplitude_color_scheme, "Сохранить выходную амплитуду", true);
}

void MainWindow::on_save_out_phase_triggered() {
    save(out_field_type::phase, out_phase_color_scheme, "Сохранить выходную фазу", true);
}

void MainWindow::on_save_oam_triggered() {
    save(out_field_type::oam, oam_color_scheme, "Сохранить плотность распределения ОУМ", true);
}

void MainWindow::on_save_all_out_distributions_triggered() {
    QString filename_ = QFileDialog::getSaveFileName(this,
                        "Сохранить все распределения",
                        QDir::currentPath(),
                        filters,
                        &defaultFilter);
    const QString filename = filename_.left(filename_.lastIndexOf('.'));
    QStringList pieces = filename_.split(".");
    QString format = pieces.value(pieces.length() - 1);
    save(filename + "_in_amplitude", format, out_field_type::amplitude, in_amplitude_color_scheme, false);
    save(filename + "_in_phase", format, out_field_type::phase, in_phase_color_scheme, false);
    save(filename + "_out_amplitude", format, out_field_type::amplitude, out_amplitude_color_scheme, true);
    save(filename + "_out_phase", format, out_field_type::phase, out_phase_color_scheme, true);
    save(filename + "_intensity", format, out_field_type::intensity, intensity_color_scheme, true);
    save(filename + "_oam", format, out_field_type::oam, oam_color_scheme, true);
}

void MainWindow::on_comboBox_activated(const QString &arg1) {
    enum hole_type old_type = hole_type;
    if (arg1 == "фазовое") {
        hole_type = hole_type::phase_hole;
    } else if (arg1 == "амплитудное") {
        hole_type = hole_type::amplitude_hole;
    } else if (arg1 == "амплитудно-фазовое") {
        hole_type = hole_type::amplitude_phase_hole;
    } else {
        hole_type = hole_type::none;
    }
    hole_.set_hole_type(hole_type);
    is_hole_type_changed = old_type != hole_type;
    if (is_hole_type_changed) {
        display_spp_and_gauss_beam();
    }
}

void MainWindow::on_settings_triggered() {
    settings->show();
}

void MainWindow::receive_size(QSize& size) {
    this->size = size;
}

void MainWindow::receive_in_amplitude_color_scheme(scheme color_scheme) {
    if (in_amplitude_color_scheme != color_scheme) {
        in_amplitude_color_scheme = color_scheme;
        display_gauss_beam();
        QString string = color_scheme_to_string(color_scheme);
        if (string != "Unknown color map") {
            QPixmap pixmap(":/scales/" + color_scheme_to_string(color_scheme) + "_192.bmp");
            ui->in_amplitude_color_scheme_label->setPixmap(pixmap);
        }
    }
}

void MainWindow::receive_in_phase_color_scheme(scheme color_scheme) {
    if (in_phase_color_scheme != color_scheme) {
        in_phase_color_scheme = color_scheme;
        display_spp();
        QString string = color_scheme_to_string(color_scheme);
        if (string != "Unknown color map") {
            QPixmap pixmap(":/scales/phase_scales/" + color_scheme_to_string(color_scheme) + "_192.bmp");
            ui->in_phase_color_scheme_label->setPixmap(pixmap);
        }
    }
}

void MainWindow::receive_spiral_color_scheme(scheme color_scheme) {
    if (spiral_color_scheme != color_scheme) {
        spiral_color_scheme = color_scheme;
        display_spiral();
        QString string = color_scheme_to_string(color_scheme);
        if (string != "Unknown color map") {
            QPixmap pixmap(":/scales/phase_scales/" + color_scheme_to_string(color_scheme) + "_192.bmp");
            ui->spiral_color_scheme_label->setPixmap(pixmap);
        }
    }
}

void MainWindow::auxiliary_function_to_process_changing_color_scheme(QLabel* image_label, QLabel* scale_label, QImage image, scheme color_scheme_to_choose, scheme& prev_color_scheme) {
    if (prev_color_scheme != color_scheme_to_choose) {
        prev_color_scheme = color_scheme_to_choose;
        QString string = color_scheme_to_string(color_scheme_to_choose);
        if (string != "Unknown color map") {
            QImage image_copy = image.copy();
            image_copy.setColorTable(load_color_map(color_scheme_to_choose));
            image_label->setPixmap(QPixmap::fromImage(image_copy));
            QString oam_prefix = scale_label == ui->oam_color_scheme_label ? "oam_scales/" : "";
            QString phase_prefix = (scale_label == ui->in_phase_color_scheme_label || scale_label == ui->out_phase_color_scheme_label) ? "phase_scales/" : "";
            QPixmap pixmap(":/scales/" + oam_prefix + phase_prefix + color_scheme_to_string(color_scheme_to_choose) + "_256.bmp");
            scale_label->setPixmap(pixmap);
        }
    }
}

void MainWindow::recieve_out_amplitude_color_scheme(scheme color_scheme) {
    auxiliary_function_to_process_changing_color_scheme(ui->amplitude_image, ui->out_amplitude_color_scheme_label, out_amplitude_cur, color_scheme, out_amplitude_color_scheme);
}

void MainWindow::receive_out_phase_color_scheme(scheme color_scheme) {
    auxiliary_function_to_process_changing_color_scheme(ui->phase_image, ui->out_phase_color_scheme_label, out_phase_cur, color_scheme, out_phase_color_scheme);
}

void MainWindow::receive_intensity_color_scheme(scheme color_scheme) {
    auxiliary_function_to_process_changing_color_scheme(ui->intensity_image, ui->intensity_color_scheme_label, intensity_cur, color_scheme, intensity_color_scheme);
}

void MainWindow::receive_oam_color_scheme(scheme color_scheme) {
    auxiliary_function_to_process_changing_color_scheme(ui->oam_density_image, ui->oam_color_scheme_label, oam_density_cur, color_scheme, oam_color_scheme);
}
