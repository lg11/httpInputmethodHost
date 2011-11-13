TEMPLATE = app
TARGET = host
;DEPENDPATH += .
;INCLUDEPATH += .

OBJECTS_DIR = tmp
MOC_DIR = tmp
CONFIG += release
;CONFIG += debug
QT += core network
;QT += core gui declarative


;unix {
    ;CONFIG += link_pkgconfig
    ;PKGCONFIG += dbus-python
;}

;INCLUDEPATH += /usr/include/python2.5
;LIBS += -L/usr/lib/python2.5 -lpython2.5

# Input
HEADERS += daemon.h http.h engine.h
SOURCES += daemon.cpp engine.cpp main.cpp
