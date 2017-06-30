#-------------------------------------------------
#
# Project created by QtCreator 2017-04-17T15:13:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Labpatch
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    floatingmenumain.cpp \
    floatingmenutools.cpp \
    submenumain.cpp \
    iconobject.cpp \
    viewportareawidget.cpp \
    executionengine.cpp

HEADERS  += mainwindow.h \
    floatingmenumain.h \
    floatingmenutools.h \
    submenumain.h \
    defines.h \
    iconobject.h \
    viewportareawidget.h \
    executionengine.h

FORMS    += mainwindow.ui \
    floatingmenumain.ui \
    floatingmenutools.ui \
    submenumain.ui

RESOURCES += \
    mainwindow.qrc

QMAKE_CXXFLAGS += -std=c++11

DISTFILES += \
    icon_classes/new file \
    scrap.txt \
    scrap/scrap.txt

win32-g++:{
    QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++
}
