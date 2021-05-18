#include "custom_validators.h"

QIntPowerOf2Validator::QIntPowerOf2Validator(QObject *parent) {
}

QValidator::State QIntPowerOf2Validator::validate (QString& input,int& pos) const {
    if (input.isEmpty()) {
        emit setError("", 1);
        return Intermediate;
    }
    bool b;
    int val = input.toInt(&b);
    if (!b) {
        emit setError(QString("Invalid integer format"), 1000);
        return Invalid;
    };
    switch (val) {
    case 1:
    case 2:
    case 4:
    case 8:
    case 16:
    case 32:
    case 64:
    case 128:
        emit setError("", 1);
        return Acceptable;
    case 3:
    case 6:
    case 12:
        emit setError(QString("The number should be in range [1;128] and should be power of 2"), 1000);
        return Intermediate;
    default:
        emit setError(QString("The number should be in range [1;128] and should be power of 2"), 1000);
        return Invalid;
    }
}
