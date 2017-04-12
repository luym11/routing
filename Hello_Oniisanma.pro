#-------------------------------------------------
#
# Project created by QtCreator 2017-04-11T17:44:17
#
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hello_Oniisanma
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    updater.cpp \
    pointmoveinformation.cpp \
    pointdoglegmoveinformation.cpp

HEADERS  += mainwindow.h \
    updater.h \
    pointmoveinformation.h \
    ../pointdoglegmoveinformation.h \
    pointdoglegmoveinformation.h

FORMS    += mainwindow.ui
