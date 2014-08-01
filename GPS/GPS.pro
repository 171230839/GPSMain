#-------------------------------------------------
#
# Project created by QtCreator 2014-07-29T21:15:00
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

TARGET = GPS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qextserialbase.cpp \
    win_qextserialport.cpp \
    worker.cpp \
    CoorTrans.cpp

HEADERS  += mainwindow.h \
    qextserialbase.h \
    win_qextserialport.h \
    worker.h \
    CoorTrans.h

FORMS    += mainwindow.ui

DESTDIR = ./
