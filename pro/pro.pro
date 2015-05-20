#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T17:38:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pro
TEMPLATE = app


SOURCES += \
    main.cpp \
    menu.cpp

HEADERS  += \
    menu.h


DISTFILES += \
    image1.png \
    image2.png \
    image3.png \
    pro.pro.user


LIBS += `pkg-config opencv --libs`

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
