QT += core gui widgets
CONFIG += c++11
TARGET = viba
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           crc.cpp \
           transmission.cpp \
           transmissiongraphicsview.cpp

HEADERS += mainwindow.h \
           crc.h \
           transmission.h \
           transmissiongraphicsview.h \


FORMS += mainwindow.ui
