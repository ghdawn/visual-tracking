#-------------------------------------------------
#
# Project created by QtCreator 2014-03-14T16:50:21
#
#-------------------------------------------------

TARGET = PostProcess
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    processimage.cpp

HEADERS += \
    processimage.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

unix:!macx: LIBS += -L$$PWD/../../../iTRLib/itrbase/bin/debug/ -litrbase

INCLUDEPATH += $$PWD/../../../iTRLib/itrbase
DEPENDPATH += $$PWD/../../../iTRLib/itrbase

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../iTRLib/itrbase/bin/debug/libitrbase.a
