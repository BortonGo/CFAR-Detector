#-------------------------------------------------
#
# Project created by QtCreator 2026-02-05T23:51:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CFARDetector
TEMPLATE = app


SOURCES += src\main.cpp\
        src\mainwindow.cpp \
        src\cfardetector.cpp \
    src/generate_cfar_test_data.cpp

HEADERS  += src\mainwindow.h \
        src\cfardetector.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

