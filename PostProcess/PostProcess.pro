#-------------------------------------------------
#
# Project created by QtCreator 2014-03-14T16:50:21
#
#-------------------------------------------------

TARGET = PostProcess
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    ../processimage.cpp

HEADERS += \
    ../processimage.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

