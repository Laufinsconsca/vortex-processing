#ifndef CUSTOM_VALIDATORS_H
#define CUSTOM_VALIDATORS_H
#include <QValidator>

class QIntPowerOf2Validator : public QValidator {
    Q_OBJECT
public:
    QIntPowerOf2Validator(QObject *parent = 0);
    virtual State validate(QString& input,int& pos) const;
signals:
    void setError(QString msg, int timeout) const;
};


#endif // CUSTOM_VALIDATORS_H
