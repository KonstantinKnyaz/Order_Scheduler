QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Orders
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        datamodel.cpp \
        tablemodel.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        datamodel.h \
        tablemodel.h \
        mainwindow.h

FORMS += \
        formEdit.ui \
        mainwindow.ui

DISTFILES += \
    printForm.html

RESOURCES +=

win32:RC_FILE = res.rc
