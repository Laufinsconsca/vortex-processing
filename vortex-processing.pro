QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_LFLAGS_RELEASE += /DEBUG /LTCG /OPT:REF /OPT:ICF
QMAKE_LFLAGS_DEBUG += /INCREMENTAL
QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,6.00

win32-msvc* {
    QMAKE_CFLAGS += /MP
    QMAKE_CXXFLAGS += /MP
    QMAKE_CFLAGS_RELEASE += /GL /Zi
    QMAKE_CXXFLAGS_RELEASE += /GL /Zi
}

TARGET = Vortex-processing
VERSION = 1.0.0
QMAKE_TARGET_PRODUCT = "Vortex-processing"
QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2021 Logachev Valentin"

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_BUNDLE=\\\"Vortex-processing\\\"
DEFINES += APP_PRODUCT=\\\"$$QMAKE_TARGET_PRODUCT\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_COMPANY=\\\"$$QMAKE_TARGET_COMPANY\\\"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    complex_amplitude.cpp \
    custom_validators.cpp \
    enums.cpp \
    gauss_beam.cpp \
    hole.cpp \
    main.cpp \
    mainwindow.cpp \
    settings.cpp \
    transformation.cpp \
    vortex.cpp

HEADERS += \
    complex_amplitude.h \
    custom_validators.h \
    enums.h \
    gauss_beam.h \
    hole.h \
    log.h \
    mainwindow.h \
    settings.h \
    transformation.h \
    vortex.h

LIBS += -lDbgHelp

FORMS += \
    mainwindow.ui \
    settings.ui

TRANSLATIONS += \
    vortex-processing_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc

DESTDIR = bin
MOC_DIR = moc
OBJECTS_DIR = obj
RCC_DIR = rcc
UI_DIR = ui

win32-msvc* {
    QMAKE_EXTRA_TARGETS += before_build makefilehook
    makefilehook.target = $(MAKEFILE)
    makefilehook.depends = .beforebuild
    PRE_TARGETDEPS += .beforebuild
    before_build.target = .beforebuild
    before_build.depends = FORCE
    before_build.commands = chcp 1251
}

