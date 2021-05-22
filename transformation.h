#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <vector>
#include <complex>

template<typename T>
class transformation {
public:
    static std::vector<T>& rotate(std::vector<T>& field_to_rotate, double angle);
};

#endif // TRANSFORMATION_H
