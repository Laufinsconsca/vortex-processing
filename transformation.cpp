#include "transformation.h"

template <class T>
std::vector<T>& transformation<T>::rotate(std::vector<T>& field_to_rotate, double angle) {
    std::vector<T> rotated_field;
    double height_2 = field_to_rotate.size() / static_cast<double>(2);
    double width_2 = field_to_rotate.at(0).size() / static_cast<double>(2);
    rotated_field.reserve(field_to_rotate.size());
    for (unsigned long long i = 0; i < field_to_rotate.size(); i++) {
        T row;
        row.reserve(field_to_rotate.at(0).size());
        for (unsigned long long j = 0; j < field_to_rotate.at(0).size(); j++) {
            unsigned long long b = static_cast<unsigned long long>(std::abs(height_2 - (i - height_2) * sin(angle) + (j - width_2) * cos(angle)));
            unsigned long long a = static_cast<unsigned long long>(std::abs(width_2 + (i - height_2) * cos(angle) + (j - width_2) * sin(angle)));
            row.push_back(a < field_to_rotate.size() && b < field_to_rotate.at(0).size() ? field_to_rotate.at(a).at(b) : 0);
        }
        rotated_field.push_back(row);
    }
    field_to_rotate.clear();
    field_to_rotate = rotated_field;
    return field_to_rotate;
}

template class transformation<std::vector<double>>;
template class transformation<std::vector<std::complex<double>>>;

