######################################################################
# Automatically generated by qmake (3.0) ?? 3? 23 12:03:41 2014
######################################################################

TEMPLATE = app
TARGET = GimbalControl
INCLUDEPATH += .

# Input
HEADERS += gimbal.h serialport.h \
    SerialSend.h
SOURCES += gimbal.cpp main.c serialport.cpp \
    SerialSend.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../iTRLib/itrbase/bin/debug/release/ -litrbase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../iTRLib/itrbase/bin/debug/debug/ -litrbase
else:unix: LIBS += -L$$PWD/../../iTRLib/itrbase/bin/debug/ -litrbase

INCLUDEPATH += $$PWD/../../iTRLib/itrbase
DEPENDPATH += $$PWD/../../iTRLib/itrbase

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../iTRLib/itrbase/bin/debug/release/itrbase.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../iTRLib/itrbase/bin/debug/debug/itrbase.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../iTRLib/itrbase/bin/debug/libitrbase.a