#-------------------------------------------------
#
# Project created by QtCreator 2018-06-30T09:39:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-client-gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        client.cpp

HEADERS += \
        client.h

FORMS += \
        client.ui

QMAKE_CFLAGS_ISYSTEM=

#Suprema scanner
unix:!macx: LIBS += -L/usr/local/lib/ -lUFScanner
unix:!macx: LIBS += -L/usr/local/lib64/ -lUFScanner
INCLUDEPATH += /usr/local/include/suprema
DEPENDPATH += /usr/local/include/suprema
INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include

#OpenCV 4.0.0-pre
unix:!macx: LIBS += -L/usr/local/lib64/ -lopencv_world
unix:!macx: LIBS += -L/usr/local/lib/ -lopencv_world
unix:!macx: LIBS += -L/usr/lib -lopencv_world
unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_world
INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include
INCLUDEPATH += /usr/include
DEPENDPATH += /usr/include

