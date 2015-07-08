#-------------------------------------------------
#
# Project created by QtCreator 2015-07-08T10:45:54
#
#-------------------------------------------------

QT       += core gui printsupport xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MoRPHED_Viewer
TEMPLATE = app


SOURCES += main.cpp\
        viewer_mainwindow.cpp \
    qcustomplot.cpp \
    customgraphicsview.cpp \
    xmlreadwrite.cpp \
    viewer_exception.cpp

HEADERS  += viewer_mainwindow.h \
    qcustomplot.h \
    customgraphicsview.h \
    xmlreadwrite.h \
    viewer_exception.h

FORMS    += viewer_mainwindow.ui
