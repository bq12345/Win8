#-------------------------------------------------
#
# Project created by QtCreator 2013-08-20T07:35:22
#
#-------------------------------------------------

QT       += core gui

TARGET = Win8
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    inputbar.cpp \
    aboutbar.cpp \
    sidebar.cpp

HEADERS  += mainwindow.h \
    sidebar.h \
    inputbar.h \
    ga.h \
    dp.h \
    Dijkstra.h \
    conway.h \
    aboutbar.h

FORMS    += mainwindow.ui \
    sidebar.ui \
    inputbar.ui \
    aboutbar.ui

RESOURCES += \
    resources.qrc \
    resources.qrc

RC_FILE = app.rc

OTHER_FILES += \
    app.rc
