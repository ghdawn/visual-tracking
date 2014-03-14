#-------------------------------------------------
#
# Project created by QtCreator 2014-03-14T17:14:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrackGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    joystickthread.cpp \
    camerathread.cpp \
    trackthread.cpp

HEADERS  += mainwindow.h \
    joystickthread.h \
    camerathread.h \
    trackthread.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$PWD/../../TrackCore/obj/Debug/ -lTrackCore

INCLUDEPATH += $$PWD/../../TrackCore
DEPENDPATH += $$PWD/../../TrackCore

unix:!macx: PRE_TARGETDEPS += $$PWD/../../TrackCore/obj/Debug/libTrackCore.a

unix:!macx: LIBS += -L$$PWD/../../../iTRLib/itrbase/bin/debug/ -litrbase

INCLUDEPATH += $$PWD/../../../iTRLib/itrbase
DEPENDPATH += $$PWD/../../../iTRLib/itrbase

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../iTRLib/itrbase/bin/debug/libitrbase.a

unix:!macx: LIBS += -L$$PWD/../../../iTRLib/itrvision/bin/debug/ -litrvision

INCLUDEPATH += $$PWD/../../../iTRLib/itrvision
DEPENDPATH += $$PWD/../../../iTRLib/itrvision

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../iTRLib/itrvision/bin/debug/libitrvision.a

unix:!macx: LIBS += -L$$PWD/../../../iTRLib/3rdparty/alglib/bin/debug/ -lalglib

INCLUDEPATH += $$PWD/../../../iTRLib/3rdparty/alglib
DEPENDPATH += $$PWD/../../../iTRLib/3rdparty/alglib

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../iTRLib/3rdparty/alglib/bin/debug/libalglib.a

unix:!macx: LIBS += -L$$PWD/../../../iTRLib/itralgorithm/bin/debug/ -litralgorithm

INCLUDEPATH += $$PWD/../../../iTRLib/itralgorithm
DEPENDPATH += $$PWD/../../../iTRLib/itralgorithm

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../iTRLib/itralgorithm/bin/debug/libitralgorithm.a

unix:!macx: LIBS += -L$$PWD/../../GimbalControl/ -lGimbalControl

INCLUDEPATH += $$PWD/../../GimbalControl
DEPENDPATH += $$PWD/../../GimbalControl

unix:!macx: PRE_TARGETDEPS += $$PWD/../../GimbalControl/libGimbalControl.a

unix:!macx: LIBS += -L$$PWD/../../../iTRLib/itrdevice/bin/debug/ -litrdevice

INCLUDEPATH += $$PWD/../../../iTRLib/itrdevice
DEPENDPATH += $$PWD/../../../iTRLib/itrdevice

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../iTRLib/itrdevice/bin/debug/libitrdevice.a

unix:!macx: LIBS += -L$$PWD/../../PostProcess/bin/debug/ -lPostProcess

INCLUDEPATH += $$PWD/../../PostProcess/PostProcess
DEPENDPATH += $$PWD/../../PostProcess/PostProcess

unix:!macx: PRE_TARGETDEPS += $$PWD/../../PostProcess/bin/debug/libPostProcess.a

unix:!macx: LIBS += -lASICamera
unix:!macx: LIBS += -lusb-1.0
