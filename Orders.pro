QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Orders
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        customdelegate.cpp \
        datamodel.cpp \
        tablemodel.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        customdelegate.h \
        datamodel.h \
        tablemodel.h \
        mainwindow.h

FORMS += \
        formEdit.ui \
        mainwindow.ui
