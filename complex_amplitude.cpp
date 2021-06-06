#include "complex_amplitude.h"

/** Default constructor */
complex_amplitude::complex_amplitude() : pixels() {}

/**
 * Copy constructor
 *
 * @param	obj	The object
 **************************************************************************************************/

complex_amplitude::complex_amplitude(const complex_amplitude& obj) : size(obj.size), pixels(obj.pixels) {}

/**
 * Constructor with the non-shifted gauss beam with sigma=0.6 as the reference beam
 *
 * @param	vortex	The vortex
 * @param	size  	The size
 **************************************************************************************************/

complex_amplitude::complex_amplitude(const class vortex& vortex, const QSize& size) :
    complex_amplitude(gauss_beam(0.6, 0, 0), vortex, size) {}

/**
 * Constructor
 *
 * @param	gauss_beam  	The gauss beam
 * @param	vortex          The vortex
 * @param	size            The size
 **************************************************************************************************/

complex_amplitude::complex_amplitude(const gauss_beam& gauss_beam, const class vortex& vortex, const QSize& size)
    : complex_amplitude(gauss_beam, vortex, size, hole{0, 0, 0, hole_type::none}) {}

/**
 * Constructor
 *
 * @exceptions	runtime_error	Raised when the amplitude image size is inconsistent with the phase image size
 *
 * @param	amplitude	The QImage amplitude
 * @param	phase		The QImage phase
 **************************************************************************************************/

complex_amplitude::complex_amplitude(QImage& amplitude, QImage& phase)
    : complex_amplitude(amplitude, phase, hole{0, 0, 0, hole_type::none}) {}

/**
 * Constructor
 *
 * @exceptions	runtime_error	Raised when the amplitude image size is inconsistent with the phase image size
 *
 * @param	amplitude	The QImage amplitude
 * @param	vortex      The vortex
 **************************************************************************************************/

complex_amplitude::complex_amplitude(QImage& amplitude, const class vortex& vortex)
    : complex_amplitude(amplitude, vortex, hole{0, 0, 0, hole_type::none}) {}

/**
 * Constructor
 *
 * @exceptions	runtime_error	Raised when the amplitude image size is inconsistent with the phase image size
 *
 * @param	gauss_beam       The gauss beam
 * @param	phase	         The QImage phase
 **************************************************************************************************/

complex_amplitude::complex_amplitude(const gauss_beam& gauss_beam, QImage& phase)
    : complex_amplitude(gauss_beam, phase, hole{0, 0, 0, hole_type::none}) {}

/**
 * Constructor
 *
 * @param	gauss_beam  	The gauss beam
 * @param	vortex          The vortex
 * @param	size            The size
 * @param   hole            The hole
 **************************************************************************************************/

complex_amplitude::complex_amplitude(const gauss_beam& gauss_beam, const class vortex& vortex, const QSize& size, const hole &hole) {
    this->size = size;
    std::vector<std::vector<double>> gauss_beam_vector_to_fill, vortex_vector_to_fill;
    std::vector<std::vector<double>> ref_beam, spp;
    if (hole.type == hole_type::amplitude_hole || hole.type == hole_type::amplitude_phase_hole) {
        ref_beam = gauss_beam::gauss_beam_to_vector(gauss_beam, gauss_beam_vector_to_fill, size, hole);
    } else {
        ref_beam = gauss_beam::gauss_beam_to_vector(gauss_beam, gauss_beam_vector_to_fill, size);
    }
    if (hole.type == hole_type::phase_hole || hole.type == hole_type::amplitude_phase_hole) {
        spp = vortex::vortex_to_vector(vortex, vortex_vector_to_fill, size, hole);
    } else {
        spp = vortex::vortex_to_vector(vortex, vortex_vector_to_fill, size);
    }
    double hx, hy;
    this->pixels.reserve(size.height());
    hx = 2. / size.width();
    hy = 2. / size.height();
    double x, y;
    for (int i = 0; i < size.height(); i++) {
        y = i * hy - 1;
        std::vector<std::complex<double>> row;
        row.reserve(size.width());
        for (int j = 0; j < size.width(); j++) {
            x = j * hx - 1;
            if (pow(x, 2) + pow(y, 2) < 1) {
                row.emplace_back(std::polar(ref_beam.at(i).at(j), spp.at(i).at(j)));
            } else {
                row.emplace_back(0);
            }
        }
        this->pixels.emplace_back(row);
    }
}

/**
 * Constructor
 *
 * @exceptions	runtime_error	Raised when the amplitude image size is inconsistent with phase image size
 *
 * @param	amplitude	The QImage amplitude
 * @param	phase		The QImage phase
 * @param   hole        The hole
 **************************************************************************************************/

complex_amplitude::complex_amplitude(QImage &amplitude, QImage &phase, const hole &hole) {
    amplitude = amplitude.convertToFormat(QImage::Format_Grayscale8);
    phase = phase.convertToFormat(QImage::Format_Grayscale8);
    QSize amplitude_size = amplitude.size();
    QSize phase_size = phase.size();
    if (hole.type == hole_type::amplitude_hole || hole.type == hole_type::amplitude_phase_hole) {
        hole::create_hole(amplitude, hole, 1);
    }
    if (hole.type == hole_type::phase_hole || hole.type == hole_type::amplitude_phase_hole) {
        hole::create_hole(phase, hole, 1);
    }
    if (amplitude_size != phase_size) {
        throw std::runtime_error("Inconsistent sizes");
    }
    size = amplitude.size();
    pixels.reserve(size.height());
    double x, y;
    for (int i = 0; i < size.height(); i++) {
        x = i - size.height()/2;
        std::vector<std::complex<double>> row;
        row.reserve(size.width());
        const unsigned char* amplitude_line = amplitude.scanLine(i);
        const unsigned char* phase_line = phase.scanLine(i);
        for (int j = 0; j < size.width(); j++) {
            y = j - size.width()/2;
            if (sqrt(x * x + y * y) < size.width() / 2) {
                row.emplace_back(std::polar(static_cast<double>(amplitude_line[j]), 2 * M_PI * static_cast<double>(phase_line[j]) / 255));
            } else {
                row.emplace_back(std::polar(0., 0.));
            }
        }
        pixels.emplace_back(row);
    }
}

void complex_amplitude::set_color_out_of_the_circle(QImage& image, QColor color) {
    double x, y;
    for (int i = 0; i < image.height(); i++) {
        x = i - image.height()/2;
        for (int j = 0; j < image.width(); j++) {
            y = j - image.width()/2;
            if (sqrt(x * x + y * y) >= image.width() / 2) {
                image.setPixelColor(i, j, color);
            }
        }
    }
}


/**
 * Constructor
 *
 * @exceptions	runtime_error	Raised when the amplitude image size is inconsistent with the phase image size
 *
 * @param	amplitude	The QImage amplitude
 * @param	vortex      The vortex
 * @param   hole        The hole
 **************************************************************************************************/

complex_amplitude::complex_amplitude(QImage &amplitude, const class vortex& vortex, const hole &hole) {
    amplitude = amplitude.convertToFormat(QImage::Format_Grayscale8);
    size = amplitude.size();
    std::vector<std::vector<double>> vortex_vector_to_fill, spp;
    if (hole.type == hole_type::amplitude_hole || hole.type == hole_type::amplitude_phase_hole) {
        hole::create_hole(amplitude, hole, 1);
    }
    if (hole.type == hole_type::phase_hole || hole.type == hole_type::amplitude_phase_hole) {
        spp = vortex::vortex_to_vector(vortex, vortex_vector_to_fill, size, hole);
    } else {
        spp = vortex::vortex_to_vector(vortex, vortex_vector_to_fill, size);
    }
    spp = vortex::vortex_to_vector(vortex, vortex_vector_to_fill, size);
    pixels.reserve(size.height());
    double x, y;
    for (int i = 0; i < size.height(); i++) {
        x = i - size.height()/2;
        std::vector<std::complex<double>> row;
        row.reserve(size.width());
        unsigned char* amplitude_line = amplitude.scanLine(i);
        for (int j = 0; j < size.width(); j++) {
            y = j - size.width()/2;
            if (sqrt(x * x + y * y) < size.width() / 2) {
                row.emplace_back(std::polar(static_cast<double>(amplitude_line[j]), spp.at(i).at(j)));
            } else {
                row.emplace_back(std::polar(0., 0.));
            }
        }
        pixels.emplace_back(row);
    }
}

/**
 * Constructor
 *
 * @exceptions	runtime_error	Raised when the amplitude image size is inconsistent with the phase image size
 *
 * @param	gauss_beam       The gauss beam
 * @param	phase	         The QImage phase
 * @param   hole             The hole
 **************************************************************************************************/

complex_amplitude::complex_amplitude(const gauss_beam& gauss_beam, QImage &phase, const hole &hole) {
    phase = phase.convertToFormat(QImage::Format_Grayscale8);
    size = phase.size();
    std::vector<std::vector<double>> gauss_beam_vector_to_fill;
    std::vector<std::vector<double>> ref_beam;
    if (hole.type == hole_type::amplitude_hole || hole.type == hole_type::amplitude_phase_hole) {
        ref_beam = gauss_beam::gauss_beam_to_vector(gauss_beam, gauss_beam_vector_to_fill, size, hole);
    } else {
        ref_beam = gauss_beam::gauss_beam_to_vector(gauss_beam, gauss_beam_vector_to_fill, size);
    }
    if (hole.type == hole_type::phase_hole || hole.type == hole_type::amplitude_phase_hole) {
        hole::create_hole(phase, hole, 1);
    }
    pixels.reserve(size.height());
    double x, y;
    for (int i = 0; i < size.height(); i++) {
        x = i - size.height()/2;
        std::vector<std::complex<double>> row;
        row.reserve(size.width());
        unsigned char* phase_line = phase.scanLine(i);
        for (int j = 0; j < size.width(); j++) {
            y = j - size.width()/2;
            if (sqrt(x * x + y * y) < size.width() / 2) {
                row.emplace_back(std::polar(ref_beam.at(i).at(j), 2 * M_PI * static_cast<double>(phase_line[j]) / 255));
            } else {
                row.emplace_back(std::polar(0., 0.));
            }
        }
        pixels.emplace_back(row);
    }
}

unsigned char* complex_amplitude::get_raw_vector(std::vector<std::vector<std::complex<double>>>& pixels, out_field_type type) {
    unsigned char* array = new unsigned char[size.width()*size.height()];
    double max_ = 0;
    if (type == out_field_type::amplitude || type == out_field_type::intensity) {
        max_ = max(type);
    }
    int i = 0;
    for (std::vector<std::complex<double>>& row : pixels) {
        for (std::complex<double> pixel : row) {
            unsigned char buf_value;
            switch (type) {
            case amplitude:
                buf_value = static_cast<unsigned char>(round(255 * abs(pixel) / max_));
                break;
            case phase:
                buf_value = static_cast<unsigned char>(round(255 * (pixel.imag() < 0 ? arg(pixel) + 2 * M_PI : arg(pixel)) * M_1_PI / 2));
                break;
            case intensity:
                buf_value = static_cast<unsigned char>(round(255 * std::norm(pixel) / max_));
                break;
            case out_field_type::oam:
                buf_value = 0; // a kind of stub
                break;
            }
            array[i++] = buf_value;
        }
    }
    return array;
}

void cleanup(void* data) {
    delete[] (unsigned char*)data;
}

QImage complex_amplitude::get_qimage(std::vector<std::vector<std::complex<double>>>& pixels, out_field_type type, scheme color_scheme) {
    unsigned char* data;
    if (type == out_field_type::oam) {
        data = get_oam_density_raw_vector(pixels);
    } else {
        data = get_raw_vector(pixels, type);
    }
    QImage image = QImage(data, size.width(), size.height(), size.width(), QImage::Format_Indexed8, cleanup);
    image.setColorTable(load_color_map(color_scheme));
    return image;
}

QImage complex_amplitude::get_qimage(out_field_type type, scheme color_scheme, QImage::Format format) {
    return get_qimage(pixels, type, color_scheme).convertToFormat(format);
}

QImage complex_amplitude::get_qimage(out_field_type type, scheme color_scheme) {
    return get_qimage(pixels, type, color_scheme);
}

QImage complex_amplitude::get_qimage(std::vector<std::vector<std::complex<double>>>& pixels, out_field_type type) {
    unsigned char* data;
    if (type == out_field_type::oam) {
        data = get_oam_density_raw_vector(pixels);
    } else {
        data = get_raw_vector(pixels, type);
    }
    QImage image = QImage(data, size.width(), size.height(), size.width(), QImage::Format_Indexed8, cleanup);
    return image;
}

QImage complex_amplitude::get_qimage(out_field_type type, QImage::Format format) {
    return get_qimage(pixels, type).convertToFormat(format);
}

QImage complex_amplitude::get_qimage(out_field_type type) {
    return get_qimage(pixels, type);
}

/**
 * Get the OAM (the total value and the density).
 *
 * @param	total_oam   The array, that contain total OAM, minimum of the OAM density image and maximum of the OAM density image (for taking possibility of the OAM density pixels negativity into account).
 *
 * @returns The reference to the QImage containing the OAM density distribution of this complex amplitude.
 **************************************************************************************************/

QImage complex_amplitude::get_oam_qimage(QVector<double> &total_oam) {
    total_oam.clear();
    unsigned char* data = get_oam_density_raw_vector(pixels);
    total_oam = this->total_oam;
    QImage oam_density = QImage(data, size.width(), size.height(), size.width(), QImage::Format_Indexed8);
    qDebug() << "oam density was formed";
    return oam_density;
}

QImage complex_amplitude::get_oam_qimage(QVector<double> &total_oam, QImage::Format format) {
    return get_oam_qimage(total_oam).convertToFormat(format);
}

QImage complex_amplitude::get_oam_qimage(QVector<double> &total_oam, scheme color_scheme) {
    total_oam.clear();
    unsigned char* data = get_oam_density_raw_vector(pixels);
    total_oam = this->total_oam;
    QImage oam_density = QImage(data, size.width(), size.height(), size.width(), QImage::Format_Indexed8);
    oam_density.setColorTable(load_color_map(color_scheme));
    return oam_density;
}

QImage complex_amplitude::get_oam_qimage(QVector<double> &total_oam, scheme color_scheme, QImage::Format format) {
    return get_oam_qimage(total_oam, color_scheme).convertToFormat(format);
}

unsigned char* complex_amplitude::get_oam_density_raw_vector(std::vector<std::vector<std::complex<double>>>& pixels) {
    std::vector<std::vector<double>> oam_pixels;
    oam_pixels.reserve(size.height());
    std::vector<std::vector<std::complex<double>>> gx, gy;
    gx = gradient('x', gx);
    gy = gradient('y', gy);
    for (int i = 0; i < size.height(); i++) {
        std::vector<double> row;
        row.reserve(size.width());
        for (int j = 0; j < size.width(); j++) {
            double width_2 = size.width() / 2.;
            double height_2 = size.height() / 2.;
            row.emplace_back(imag(conj(pixels.at(i).at(j)) * (gy.at(i).at(j) * (j - width_2) - gx.at(i).at(j) * (i - height_2))));
        }
        oam_pixels.emplace_back(row);
    }
    double oam_numerator = 0, oam_denominator = 0;
    for (int i = 0; i < size.height(); i++) {
        for (int j = 0; j < size.width(); j++) {
            oam_numerator += oam_pixels.at(i).at(j);
            oam_denominator += std::norm(pixels.at(i).at(j));
        }
    }
    double min_oam_density = 1e308;
    double max_oam_density = -1e308;
    for (std::vector<double>& row : oam_pixels) {
        for (double pixel : row) {
            if (min_oam_density > pixel) {
                min_oam_density = pixel;
            }
        }
    }
    for (std::vector<double>& row : oam_pixels) {
        for (double pixel : row) {
            if (pixel > max_oam_density) {
                max_oam_density = pixel;
            }
        }
    }
    double min_by_abs_oam_density = abs(min_oam_density) < abs(max_oam_density) ? min_oam_density : max_oam_density;
    double max_by_abs_oam_density = min_by_abs_oam_density == min_oam_density ? max_oam_density : min_oam_density;
    for (int i = 0; i <  static_cast<int>(oam_pixels.size()); i++) {;
        for (int j = 0; j < static_cast<int>(oam_pixels.at(0).size()); j++) {
            oam_pixels.at(i).at(j) -= min_by_abs_oam_density;
        }
    }
    double max = max_by_abs_oam_density - min_by_abs_oam_density;
    for (int i = 0; i < static_cast<int>(oam_pixels.size()); i++) {
        for (int j = 0; j < static_cast<int>(oam_pixels.at(0).size()); j++) {
            oam_pixels.at(i).at(j) *= 255 / max * (min_by_abs_oam_density == min_oam_density ? 1 : -1);
            oam_pixels.at(i).at(j) += min_by_abs_oam_density == min_oam_density ? 0 : 255;
        }
    }
    unsigned char* array = new unsigned char[size.width()*size.height()];
    int i = 0;
    for (std::vector<double>& row : oam_pixels) {
        for (double pixel : row) {
            array[i++] = static_cast<unsigned char>(round(pixel));
        }
    }
    total_oam.append(oam_numerator / oam_denominator);
    total_oam.append(min_oam_density - min_by_abs_oam_density);
    total_oam.append(max_oam_density - min_by_abs_oam_density);
    return array;
}

QVector<double> complex_amplitude::get_total_oam() {
    total_oam.clear();
    unsigned char* buf_pixels = get_oam_density_raw_vector(pixels);
    delete[] buf_pixels;
    return total_oam;
}


void complex_amplitude::write(QString filename, const char* format, out_field_type type, scheme color_scheme) {
    get_qimage(type, color_scheme).save(filename, format, 100);
}

/**
 * Find the max of the whole complex amplitude normalized to given field type.
 *
 * @param	type	The out field type.
 *
 * @returns         The maximum.
 **************************************************************************************************/

double complex_amplitude::max(out_field_type type) {
    double max = -1;
    for (std::vector<std::complex<double>>& row : pixels) {
        for (std::complex<double> pixel : row) {
            switch (type) {
            case out_field_type::amplitude: {
                if (max < abs(pixel)) {
                    max = abs(pixel);
                }
                break;
            }
            case out_field_type::intensity: {
                if (max < abs(pixel) * abs(pixel)) {
                    max = abs(pixel) * abs(pixel);
                }
                break;
            }
            case out_field_type::phase: {
                return 2*M_PI;
            }
            default: {
                return 0; // a kind of stub
            }
            }
        }
    }
    return max;
}

/**
 * Find the min of the whole complex amplitude normalized to given field type.
 *
 * @param	type	The out field type.
 *
 * @returns         The minimum.
 **************************************************************************************************/

double complex_amplitude::min(out_field_type type) {
    double min = 1e308;
    for (std::vector<std::complex<double>>& row : pixels) {
        for (std::complex<double> pixel : row) {
            switch (type) {
            case out_field_type::amplitude: {
                if (min > abs(pixel)) {
                    min = abs(pixel);
                }
                break;
            }
            case out_field_type::intensity: {
                if (min > abs(pixel) * abs(pixel)) {
                    min = abs(pixel) * abs(pixel);
                }
                break;
            }
            case out_field_type::phase: {
                return 0;
            }
            default: {
                return 0; // a kind of stub
            }
            }
        }
    }
    return min;
}

/**
 * 1-dimension fast Fourier transform.
 *
 * @exceptions	runtime_error	Raised when the size of this object is not power of 2.
 *
 * @param	dir				   	The direction of the transform (1 for direct, -1 for inverse).
 * @param	size			   	The size.
 * @param	vector_to_transform	A vector to tranform.
 *
 * @returns                     The reference to the transformed vector.
 **************************************************************************************************/

std::vector<std::complex<double>>& complex_amplitude::FFT1D(int dir, int size, std::vector<std::complex<double>>& vector_to_transform) {
    int i, k, m = 0;
    for (i = 1; i < size; i <<= 1, m++);
    if (i != size) {
        throw std::runtime_error("Invalid size");
    }
    float q = 1.;
    for (i = 0; i < size; i++, q = -q) {
        vector_to_transform.at(i) *= q;
    }
    int n_2 = size / 2, n_1 = size - 1, j = 0;
    for (i = 0; i < n_1; i++) {
        if (i < j) {
            std::complex<double> t = vector_to_transform.at(i);
            vector_to_transform.at(i) = vector_to_transform.at(j);
            vector_to_transform.at(j) = t;
        }
        k = n_2;
        while (k <= j) {
            j -= k;
            k >>= 1;
        }
        j += k;
    }

    int le = 1;
    float c_M_PI = dir * M_PI;
    for (int l = 0; l < m; l++) {
        int le1 = le;
        le *= 2;
        std::complex<double> u = 1.;
        std::complex<double> w = std::polar(1., (double)(-c_M_PI / le1));
        for (j = 0; j < le1; j++, u *= w) {
            for (i = j; i < size; i += le) {
                int ip = i + le1;
                std::complex<double> t = vector_to_transform.at(ip) * u;
                vector_to_transform.at(ip) = vector_to_transform.at(i) - t;
                vector_to_transform.at(i) += t;
            }
        }
    }
    for (i = 0, q = 1. / (dir == 1 ? size : 1); i < size; i++, q = -q) {
        vector_to_transform.at(i) *= q;
    }
    return vector_to_transform;
}

/**
 * 2-dimension fast Fourier transform.
 *
 * @exceptions	runtime_error	Raised when the size of this object is not power of 2.
 *
 * @param	dir                 The direction of the transform (1 for direct, -1 for inverse).
 * @param	expansion           The expansion (should be power of 2).
 **************************************************************************************************/

void complex_amplitude::_FFT2D(int dir, int expansion) {
    if (expansion == -1) {
        throw std::runtime_error("The FFT expansion text field must contain a power-of-2 integer value");
    }
    if (!is_power_of_2(expansion)) {
        throw std::runtime_error("The FFT expansion must be power of 2");
    }
    if (!is_power_of_2(size)) {
        throw std::runtime_error("The image size must be power of 2");
    }
    int i, j, t;
    t = 0;
    do {
        t++;
    } while ((1 << t) < size.height());
    t = 1 << t;
    int line_size = t * expansion;
    int st = (line_size - size.width()) / 2;
    int end = st + size.width();
    std::vector<std::complex<double>> line(line_size);
    for (i = 0; i < size.height(); i++) {
        for (j = 0; j < line_size; j++) {
            if ((j >= st) && (j < end)) {
                line.at(j) = pixels.at(i).at(j - st);
            } else {
                line.at(j) = 0;
            }
        }
        line = FFT1D(dir, line_size, line);
        for (j = 0; j < size.width(); j++) {
            pixels.at(i).at(j) = line.at(j + st);
        }
    }
    line.clear();

    t = 0;
    do {
        t++;
    } while ((1 << t) < size.width());
    t = 1 << t;
    line_size = t * expansion;
    st = (line_size - size.height()) / 2;
    end = st + size.height();
    line = std::vector<std::complex<double>>(line_size);

    for (i = 0; i < size.width(); i++) {
        for (j = 0; j < line_size; j++) {
            if ((j >= st) && (j < end)) {
                line.at(j) = pixels.at(j - st).at(i);
            } else {
                line.at(j) = 0;
            }
        }
        line = FFT1D(dir, line_size, line);
        for (j = 0; j < size.height(); j++) {
            pixels.at(j).at(i) = line.at(j + st);
        }
    }
    line.clear();
}

/**
 * 2-dimension direct fast Fourier transform.
 *
 * @param	expansion	The expansion (should be power of 2, allows you to increase an output Fourier transform image).
 **************************************************************************************************/

void complex_amplitude::FFT2D(int expansion) {
    _FFT2D(1, expansion);
}

/**
 * 2-dimension inverse fast Fourier transform.
 *
 * @param	expansion	The expansion (should be power of 2, allows you to increase an output Fourier transform image).
 **************************************************************************************************/

void complex_amplitude::IFFT2D(int expansion) {
    _FFT2D(-1, expansion);
}


/**
 * Get gradient of this for given variable.
 *
 * @param	var                 The variable symbol that indicates direction ('x' or 'y').
 * @param  grad                 A reference to a vector to contain the gradient in the chosen direction.
 *
 * @exception	runtime_error	Raised when variable is undefined (isn't 'x' or 'y').
 *
 * @returns                     The reference to a vector that contains the gradient in the chosen direction.
 **************************************************************************************************/

std::vector<std::vector<std::complex<double>>>& complex_amplitude::gradient(char var, std::vector<std::vector<std::complex<double>>>& grad) {
    grad.clear();
    switch (var) {
    case 'x': {
        grad.reserve(size.height());
        for (int i = 0; i < size.height(); i++) {
            std::vector<std::complex<double>> row;
            row.reserve(size.width());
            for (int j = 0; j < size.width(); j++) {
                if (j != size.width() - 1) {
                    row.emplace_back(pixels.at(i).at(j) - pixels.at(i).at(j + 1));
                } else {
                    row.emplace_back(0);
                }
            }
            grad.emplace_back(row);
        }
        break;
    }
    case 'y': {
        grad.reserve(size.height());
        for (int i = 0; i < size.height(); i++) {
            std::vector<std::complex<double>> row;
            row.reserve(size.width());
            for (int j = 0; j < size.width(); j++) {
                if (i != size.height() - 1) {
                    row.emplace_back(pixels.at(i).at(j) - pixels.at(i + 1).at(j));
                } else {
                    row.emplace_back(0);
                }
            }
            grad.emplace_back(row);
        }
        break;
    }
    default:
        throw std::runtime_error("Undefined variable to get the gradient of");
    }
    return grad;
}

bool complex_amplitude::is_power_of_2(QSize& size) {
    int greater_value = size.height() > size.width() ? size.height() : size.width();
    int less_value = greater_value == size.width() ? size.height() : size.width();
    double m = 0.5;
    bool is_less_value_appropriate = false;
    bool ais_greater_value_appropriate = false;
    do {
        m *= 2;
        if (greater_value == m) {
            ais_greater_value_appropriate = true;
        }
        if (less_value == m) {
            is_less_value_appropriate = true;
        }
    } while (2 * m <= greater_value);
    return is_less_value_appropriate && ais_greater_value_appropriate;
};

bool complex_amplitude::is_power_of_2(int value) {
    double m = 0.5;
    bool is_value_appropriate = false;
    do {
        m *= 2;
        if (value == m) {
            is_value_appropriate = true;
        }
    } while (2 * m <= value);
    return is_value_appropriate;
}

complex_amplitude& complex_amplitude::operator=(const complex_amplitude& obj) {
    if (this == &obj) {
        return *this;
    }
    pixels = obj.pixels;
    size = obj.size;
    total_oam = obj.total_oam;
    return *this;
}
