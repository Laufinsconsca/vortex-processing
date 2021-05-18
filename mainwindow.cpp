#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    settings = new Settings();
    settings->setWindowModality(Qt::ApplicationModal);
    connect(settings, &Settings::send_size, this, &MainWindow::recieve_size);
    connect(settings, &Settings::send_in_amplitude_color_scheme, this, &MainWindow::recieve_in_amplitude_color_scheme);
    connect(settings, &Settings::send_in_phase_color_scheme, this, &MainWindow::recieve_in_phase_color_scheme);
    connect(settings, &Settings::send_out_amplitude_color_scheme, this, &MainWindow::recieve_out_amplitude_color_scheme);
    connect(settings, &Settings::send_out_phase_color_scheme, this, &MainWindow::recieve_out_phase_color_scheme);
    connect(settings, &Settings::send_intensity_color_scheme, this, &MainWindow::recieve_intensity_color_scheme);
    connect(settings, &Settings::send_oam_color_scheme, this, &MainWindow::recieve_oam_color_scheme);
    connect(ui->m_line, &QLineEdit::editingFinished, this, &MainWindow::display_spp);
    connect(ui->fi_line, &QLineEdit::editingFinished, this, &MainWindow::display_spp);
    connect(ui->r_d_line, &QLineEdit::editingFinished, this, &MainWindow::display_both);
    connect(ui->r_hole_line, &QLineEdit::editingFinished, this, &MainWindow::display_both);
    connect(ui->r_fi_line, &QLineEdit::editingFinished, this, &MainWindow::display_both);
    connect(ui->sigma_line, &QLineEdit::editingFinished, this, &MainWindow::display_gauss_beam);
    connect(ui->shift_line, &QLineEdit::editingFinished, this, &MainWindow::display_gauss_beam);
    ui->m_line->setValidator(new QIntValidator(-100, 100, this));
    ui->fi_line->setValidator(new QDoubleValidator(0, 10, 3, this));
    ui->r_fi_line->setValidator(new QIntValidator(0, 360, this));
    ui->sigma_line->setValidator(new QDoubleValidator(0, 5, 3, this));
    ui->shift_line->setValidator(new QDoubleValidator(-1, 1, 3, this));
    ui->r_d_line->setValidator(new QDoubleValidator(0, 1, 3, this));
    ui->r_hole_line->setValidator(new QDoubleValidator(0, 1, 3, this));
    QFont font = ui->total_oam_label->font();
    font.setPointSize(12);
    ui->total_oam_label->setFont(font);
    QIntPowerOf2Validator* validator = new QIntPowerOf2Validator(this);
    connect(validator,  SIGNAL(setError(QString,int)),
            ui->statusbar, SLOT(showMessage(QString,int)));
    ui->fft_expansion_line->setValidator(validator);
    on_fft_expansion_line_editingFinished();
    /* to display the initial phase and gauss distributions*/
    display_both();
}

MainWindow::~MainWindow() {
    delete ui;
}

complex_amplitude& MainWindow::read_complex_amplitude(complex_amplitude& complex_amplitude_, QSize& size) {
    if (!is_amplitude_from_file && !is_phase_from_file) {
        class vortex vortex(m, fi);
        gauss_beam gauss_beam(sigma, shift);
        complex_amplitude_ = complex_amplitude(gauss_beam, vortex, size, hole_);
    } else if (is_amplitude_from_file && !is_phase_from_file) {
        QImage amplitude_image = amplitude_from_file.copy().scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        class vortex vortex(m, fi);
        complex_amplitude_ = complex_amplitude(amplitude_image, vortex, hole_);
    } else if (!is_amplitude_from_file && is_phase_from_file) {
        gauss_beam gauss_beam(sigma, shift);
        QImage phase_image = phase_from_file.copy().scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        complex_amplitude_ = complex_amplitude(gauss_beam, phase_image, hole_);
    } else {
        QImage amplitude_image = amplitude_from_file.copy().scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QImage phase_image = phase_from_file.copy().scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        complex_amplitude_ = complex_amplitude(amplitude_image, phase_image, hole_);
    }
    return complex_amplitude_;
}

void MainWindow::on_fft_clicked() {
    QSize size(256,256);
    complex_amplitude complex_amplitude_;
    complex_amplitude_ = read_complex_amplitude(complex_amplitude_, size);
    complex_amplitude_.FFT2D(fft_expansion);
    intensity_cur = complex_amplitude_.get_qimage(out_field_type::intensity, scheme::gray);
    out_amplitude_cur = complex_amplitude_.get_qimage(out_field_type::amplitude, scheme::gray);
    out_phase_cur = complex_amplitude_.get_qimage(out_field_type::phase, scheme::gray);
    QImage out_amplitude_to_show = out_amplitude_cur.copy();
    QImage out_phase_to_show = out_phase_cur.copy();
    QImage intensity_to_show = intensity_cur.copy();
    out_amplitude_to_show.setColorTable(color_map(out_amplitude_color_scheme));
    out_phase_to_show.setColorTable(color_map(out_phase_color_scheme));
    intensity_to_show.setColorTable(color_map(intensity_color_scheme));
    ui->amplitude_image->setPixmap(QPixmap::fromImage(out_amplitude_to_show));
    ui->phase_image->setPixmap(QPixmap::fromImage(out_phase_to_show));
    ui->intensity_image->setPixmap(QPixmap::fromImage(intensity_to_show));
}

void MainWindow::on_find_oam_clicked() {

    FUNCTION_LOG

    QSize size(256,256);
    complex_amplitude complex_amplitude_;
    complex_amplitude_ = read_complex_amplitude(complex_amplitude_, size);
    complex_amplitude_.FFT2D(fft_expansion);
    QVector<double> total_oam;
    oam_density_cur = complex_amplitude_.get_oam_qimage(total_oam, scheme::gray);
    QImage oam_density_to_show = oam_density_cur.copy();
    oam_density_to_show.setColorTable(color_map(oam_color_scheme));
    ui->oam_density_image->setPixmap(QPixmap::fromImage(oam_density_to_show));
    ui->total_oam_label->setText("Общее значение ОУМ: " + QString(std::to_string(total_oam.at(0)).c_str()));
}

void MainWindow::on_load_amplitude_triggered() {
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Загрузить амплитуду"),
                       "L:/",
                       tr("Изображения (*.png *.bmp *.jpg)"));
    if (!filename.isEmpty()) {
        QPixmap pixmap(filename);
        amplitude_from_file = pixmap.toImage();
        ui->gauss_image->setPixmap(pixmap);
        is_amplitude_from_file = true;
    }
}

void MainWindow::on_load_phase_triggered() {
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Загрузить фазу"),
                       "L:/",
                       tr("Изображения (*.png *.bmp *.jpg)"));
    if (!filename.isEmpty()) {
        QPixmap pixmap(filename);
        phase_from_file = pixmap.toImage();
        ui->spp_image->setPixmap(pixmap);
        ui->spp_image->show();
        is_phase_from_file = true;
    }
}

void MainWindow::display_spp() {
    if (spp_param_preprocessing() && holes_param_preprocessing()) {
        QSize size(256, 256);
        class vortex vortex(m, fi);
        gauss_beam gauss_beam(0.6, 0); // gauss parameters are no matter
        complex_amplitude a_vortex(gauss_beam, vortex, size, hole_);
        QImage temp = a_vortex.get_qimage(out_field_type::phase, in_phase_color_scheme).copy();
        ui->spp_image->setPixmap(QPixmap::fromImage(temp));
        is_phase_from_file = false;
    }
}

void MainWindow::display_gauss_beam() {
    if (gauss_param_preprocessing() && holes_param_preprocessing()) {
        QSize size(256, 256);
        class vortex vortex(1, 1); // vortex parameters are no matter
        gauss_beam gauss_beam(sigma, shift);
        complex_amplitude a_vortex(gauss_beam, vortex, size, hole_);
        ui->gauss_image->setPixmap(QPixmap::fromImage(a_vortex.get_qimage(out_field_type::amplitude, in_amplitude_color_scheme).copy()));
        is_amplitude_from_file = false;
    }
}

void MainWindow::display_both() {
    if (holes_param_preprocessing()) {
        if (!is_phase_from_file) {
            spp_param_preprocessing();
        }
        if (!is_amplitude_from_file) {
            gauss_param_preprocessing();
        }
        complex_amplitude complex_amplitude_;
        complex_amplitude_ = read_complex_amplitude(complex_amplitude_, size);
        if (is_phase_from_file) {
            ui->spp_image->setPixmap(QPixmap::fromImage(complex_amplitude_.get_qimage(out_field_type::phase)));
        } else {
            ui->spp_image->setPixmap(QPixmap::fromImage(complex_amplitude_.get_qimage(out_field_type::phase, in_phase_color_scheme)));
        }
        if (is_amplitude_from_file) {
            ui->gauss_image->setPixmap(QPixmap::fromImage(complex_amplitude_.get_qimage(out_field_type::amplitude)));
        } else {
            ui->gauss_image->setPixmap(QPixmap::fromImage(complex_amplitude_.get_qimage(out_field_type::amplitude, in_amplitude_color_scheme)));
        }
    }
}

bool MainWindow::spp_param_preprocessing() {
    if (ui->m_line->text().isEmpty() || ui->fi_line->text().isEmpty()) {
        return false;
    }
    bool m_label_to_string_ok, fi_label_to_string_ok;
    double m = ui->m_line->text().toDouble(&m_label_to_string_ok);
    double fi = ui->fi_line->text().toDouble(&fi_label_to_string_ok);
    if (m_label_to_string_ok && fi_label_to_string_ok) {
        this->m = m;
        this->fi = fi;
        return true;
    } else {
        return false;
    }
}

bool MainWindow::gauss_param_preprocessing() {
    if (ui->sigma_line->text().isEmpty() || ui->shift_line->text().isEmpty()) {
        return false;
    }
    bool sigma_label_to_string_ok, shift_label_to_string_ok;
    double sigma = ui->sigma_line->text().toDouble(&sigma_label_to_string_ok);
    double shift = ui->shift_line->text().toDouble(&shift_label_to_string_ok);
    if (sigma_label_to_string_ok && shift_label_to_string_ok) {
        this->sigma = sigma;
        this->shift = shift;
        return true;
    } else {
        return false;
    }
}

bool MainWindow::holes_param_preprocessing() {

    FUNCTION_LOG

    if (hole_type == hole_type::none) { // если нет отверстия, то false
        return true;
    }
    if (ui->r_d_line->text().isEmpty() // если строки пустые, то false
            || ui->r_hole_line->text().isEmpty()
            || ui->r_fi_line->text().isEmpty()) {
        return false;
    }
    bool r_d_label_to_string_ok, r_hole_label_to_string_ok, r_fi_label_to_string_ok;
    double r_d = ui->r_d_line->text().toDouble(&r_d_label_to_string_ok);
    double r_hole = ui->r_hole_line->text().toDouble(&r_hole_label_to_string_ok);
    if (r_d_label_to_string_ok
            && r_hole_label_to_string_ok) {
        if (r_d > 1 || r_hole > 1) { // для перестраховки, валидатор уже есть
            return false;
        }
        QLineEdit* line = qobject_cast<QLineEdit*>(sender());
        if (r_d + r_hole > 1) {
            if (line == ui->r_hole_line) {
                r_hole = 1 - r_d;
                ui->r_hole_line->setText(QString::number(r_hole));
            } else if (line == ui->r_d_line) {
                r_d = 1 - r_hole;
                ui->r_d_line->setText(QString::number(r_d));
            }
            return false;
        }
        double r_fi = ui->r_fi_line->text().toDouble(&r_fi_label_to_string_ok);
        if (r_fi_label_to_string_ok
                && (is_hole_type_changed
                    || (hole_.r_d != r_d)
                    || (hole_.r_hole != r_hole)
                    || (hole_.fi != r_fi))) {
            if (hole_type != hole_type::none && r_hole == 0) { // если радиус отверстия 0, то false
                return false;
            }
            this->hole_ = hole(r_d, r_hole, r_fi, hole_type);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
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

void MainWindow::save(out_field_type type, scheme color_scheme, QString description) {

    FUNCTION_LOG

    QString filename = QFileDialog::getSaveFileName(this,
                       description,
                       "L:/",
                       filters,
                       &defaultFilter);
    if (!filename.isEmpty()) {
        complex_amplitude complex_amplitude_;
        complex_amplitude_ = read_complex_amplitude(complex_amplitude_, size);
        QImage image;
        complex_amplitude_.FFT2D(fft_expansion);
        QStringList pieces = filename.split(".");
        QString format = pieces.value(pieces.length() - 1);
        if (type == out_field_type::oam) {
            QVector<double> total_oam;
            image = complex_amplitude_.get_oam_qimage(total_oam, color_scheme);
            const QString directory = filename.left(filename.lastIndexOf('/'));
            QFile total_oam_file(directory + "oam.oam");
            QFile min_oam(directory + "oam.min");
            QFile max_oam(directory + "oam.max");
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
        }
        image.save(filename, format.toStdString().c_str());
    }
}

void MainWindow::on_save_intensity_triggered() {
    save(out_field_type::intensity, intensity_color_scheme, "Сохранить интенсивность");
}

void MainWindow::on_save_amplitude_triggered() {
    save(out_field_type::amplitude, out_amplitude_color_scheme, "Сохранить амплитуду");
}

void MainWindow::on_save_phase_triggered() {
    save(out_field_type::phase, out_phase_color_scheme, "Сохранить фазу");
}

void MainWindow::on_save_oam_triggered() {
    save(out_field_type::oam, oam_color_scheme, "Сохранить плотность распределения ОУМ");
}

void MainWindow::on_comboBox_textActivated(const QString &arg1) {
    enum hole_type old_type = hole_type;
    if (arg1 == "фазовое") {
        hole_type = hole_type::phase_hole;
    } else if (arg1 == "амплитудное") {
        hole_type = hole_type::amplitude_hole;
    } else if (arg1 == "фазоамплитудное") {
        hole_type = hole_type::amplitude_phase_hole;
    } else {
        hole_type = hole_type::none;
    }
    hole_.set_hole_type(hole_type);
    is_hole_type_changed = old_type != hole_type;
    if (is_hole_type_changed) {
        display_both();
    }
}

void MainWindow::on_settings_triggered() {
    settings->show();
}

void MainWindow::recieve_size(QSize& size) {
    this->size = size;
}

void MainWindow::recieve_in_amplitude_color_scheme(scheme color_scheme) {
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

void MainWindow::recieve_in_phase_color_scheme(scheme color_scheme) {
    if (in_phase_color_scheme != color_scheme) {
        in_phase_color_scheme = color_scheme;
        display_spp();
        QString string = color_scheme_to_string(color_scheme);
        if (string != "Unknown color map") {
            QPixmap pixmap(":/scales/" + color_scheme_to_string(color_scheme) + "_192.bmp");
            ui->in_phase_color_scheme_label->setPixmap(pixmap);
        }
    }
}

void MainWindow::auxiliary_function_to_process_changing_color_scheme(QLabel* image_label, QLabel* scale_label, QImage image, scheme color_scheme_to_choose, scheme& prev_color_scheme) {
    if (prev_color_scheme != color_scheme_to_choose) {
        prev_color_scheme = color_scheme_to_choose;
        QString string = color_scheme_to_string(color_scheme_to_choose);
        if (string != "Unknown color map") {
            QImage image_copy = image.copy();
            image_copy.setColorTable(color_map(color_scheme_to_choose));
            image_label->setPixmap(QPixmap::fromImage(image_copy));
            QPixmap pixmap(":/scales/" + color_scheme_to_string(color_scheme_to_choose) + "_256.bmp");
            scale_label->setPixmap(pixmap);
        }
    }
}

void MainWindow::recieve_out_amplitude_color_scheme(scheme color_scheme) {
    auxiliary_function_to_process_changing_color_scheme(ui->amplitude_image, ui->out_amplitude_color_scheme_label, out_amplitude_cur, color_scheme, out_amplitude_color_scheme);
}

void MainWindow::recieve_out_phase_color_scheme(scheme color_scheme) {
    auxiliary_function_to_process_changing_color_scheme(ui->phase_image, ui->out_phase_color_scheme_label, out_phase_cur, color_scheme, out_phase_color_scheme);
}

void MainWindow::recieve_intensity_color_scheme(scheme color_scheme) {
    auxiliary_function_to_process_changing_color_scheme(ui->intensity_image, ui->intensity_color_scheme_label, intensity_cur, color_scheme, intensity_color_scheme);
}

void MainWindow::recieve_oam_color_scheme(scheme color_scheme) {
    auxiliary_function_to_process_changing_color_scheme(ui->oam_density_image, ui->oam_color_scheme_label, oam_density_cur, color_scheme, oam_color_scheme);
}