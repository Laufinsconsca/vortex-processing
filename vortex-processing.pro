QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    complex_amplitude.cpp \
    curve.cpp \
    custom_validators.cpp \
    enums.cpp \
    gauss_beam.cpp \
    hole.cpp \
    main.cpp \
    mainwindow.cpp \
    polygonal_spiral.cpp \
    settings.cpp \
    spiral.cpp \
    transformation.cpp \
    vortex.cpp

HEADERS += \
    complex_amplitude.h \
    curve.h \
    custom_validators.h \
    enums.h \
    gauss_beam.h \
    hole.h \
    log.h \
    mainwindow.h \
    polygonal_spiral.h \
    settings.h \
    spiral.h \
    transformation.h \
    vortex.h

FORMS += \
    mainwindow.ui \
    settings.ui

TRANSLATIONS += \
    vortex-processing_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    vortex-processing.pro.user

RESOURCES += \
    resources/resources.qrc
