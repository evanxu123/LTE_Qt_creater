#-------------------------------------------------
#
# Project created by QtCreator 2014-11-01T19:04:52
#
#-------------------------------------------------

QT       += core gui
QT     += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project8
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    constellationdiagram.cpp \
    estimating_diagram.cpp \
    showThread.cpp \
    snr.cpp \
    est_phase.cpp

HEADERS  += mainwindow.h \
    H/mainwindow.h \
    struct_type.h \
    H/dialog.h \
    mythread.h \
    constellationdiagram.h \
    estimating_diagram.h \
    showThread.h \
    snr.h \
    est_phase.h

FORMS    += mainwindow.ui \
    dialog.ui \
    constellationdiagram.ui \
    estimating_diagram.ui \
    snr.ui \
    est_phase.ui
CONFIG += console
