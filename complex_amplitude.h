#ifndef COMPLEX_AMPLITUDE_H
#define COMPLEX_AMPLITUDE_H

#include <QtMath>
#include <vector>
#include <complex>
#include <QString>
#include <QByteArray>
#include <QImage>
#include <QImageReader>
#include <QBuffer>
#include <QDebug>
#include <QVector>
#include "enums.h"
#include "vortex.h"
#include "hole.h"
#include "gauss_beam.h"
#include "log.h"

class complex_amplitude {
public:
    QSize size;
    complex_amplitude();
    complex_amplitude(const complex_amplitude& obj);
    complex_amplitude(const class vortex& vortex, const QSize& size);
    complex_amplitude(const gauss_beam& gauss_beam_, const class vortex& vortex_, const QSize& size);
    complex_amplitude(const gauss_beam& gauss_beam, QImage& phase);
    complex_amplitude(const gauss_beam& gauss_beam, const class vortex& vortex_, const QSize& size, const hole& hole);
    complex_amplitude(const gauss_beam& gauss_beam_, QImage& phase, const hole& hole);
    complex_amplitude(QImage& amplitude, QImage& phase);
    complex_amplitude(QImage& amplitude, const class vortex& vortex_);
    complex_amplitude(QImage& amplitude, QImage& phase, const hole& hole);
    complex_amplitude(QImage& amplitude, const class vortex& vortex_, const hole& hole);
    QImage get_qimage(out_field_type type, scheme color_scheme, QImage::Format format);
    QImage get_qimage(out_field_type type, scheme color_scheme);
    QImage get_qimage(out_field_type type, QImage::Format format);
    QImage get_qimage(out_field_type type);
    QImage get_oam_qimage(QVector<double>& total_oam);
    QImage get_oam_qimage(QVector<double>& total_oam, QImage::Format format);
    QImage get_oam_qimage(QVector<double>& total_oam, scheme color_scheme);
    QImage get_oam_qimage(QVector<double>& total_oam, scheme color_scheme, QImage::Format format);
    QVector<double> get_total_oam();
    void write(QString filename, const char* format, out_field_type type, scheme color_scheme);
    void FFT2D(int expansion);
    void IFFT2D(int expansion);
    static bool is_power_of_2(int value);
    complex_amplitude& operator=(const complex_amplitude& obj);
    static void set_color_out_of_the_circle(QImage& image, QColor color);
private:
    std::vector<std::vector<std::complex<double>>> pixels;
    QVector<double> total_oam;
    hole none_hole{0, 0, 0, hole_type::none}; // the stub
    unsigned char* get_raw_vector(std::vector<std::vector<std::complex<double>>>& pixels, out_field_type type);
    unsigned char* get_oam_density_raw_vector(std::vector<std::vector<std::complex<double>>>& pixels);
    QImage get_qimage(std::vector<std::vector<std::complex<double>>>& pixels, out_field_type type, scheme color_scheme);
    QImage get_qimage(std::vector<std::vector<std::complex<double>>>& pixels, out_field_type type);
    void _FFT2D(int dir, int expansion);
    std::vector<std::complex<double>>& FFT1D(int dir, int m, std::vector<std::complex<double>>& transforming_vector);
    static bool is_power_of_2(QSize& size);
    double max(out_field_type type);
    double min(out_field_type type);
    std::vector<std::vector<std::complex<double>>>& gradient(char var, std::vector<std::vector<std::complex<double>>>& grad);
};

#endif // COMPLEX_AMPLITUDE_H
