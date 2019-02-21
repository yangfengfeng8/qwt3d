#-------------------------------------------------
#
# Project created by QtCreator 2019-02-21T09:51:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qwt3d
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    qwt3d_plot.cpp \
    qwt3d_types.cpp

HEADERS  += widget.h \
    qwt3d_plot.h \
    qwt3d_autoptr.h \
    qwt3d_enrichment.h \
    qwt3d_types.h \
    qwt3d_portability.h \
    qwt3d_helper.h \
    qwt3d_openglhelper.h

FORMS    += widget.ui
