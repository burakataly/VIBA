
QT += widgets
CONFIG += c++17
TEMPLATE = app
TARGET = viba

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    crc.cpp \
    frame.cpp \
    transmission.cpp \
    transmissiongraphicsview.cpp

HEADERS += \
    crc.h \
    frame.h \
    mainwindow.h \
    transmission.h \
    transmissiongraphicsview.h

FORMS += mainwindow.ui
