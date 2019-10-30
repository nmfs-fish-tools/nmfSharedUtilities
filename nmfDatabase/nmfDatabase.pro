#-------------------------------------------------
#
# Project created by QtCreator 2017-03-04T00:15:30
#
#-------------------------------------------------

QT       += sql core gui widgets

TARGET = nmfDatabase
TEMPLATE = lib

CONFIG += c++14

DEFINES += NMFDATABASE_LIBRARY
QMAKE_CXXFLAGS += -std=c++0x

SOURCES += nmfDatabase.cpp \
    nmfDatabaseTableDefinitions.cpp

HEADERS += nmfDatabase.h \
    nmfdatabase_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-nmfUtilities-Qt_5_12_3_gcc64-Release/release/ -lnmfUtilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-nmfUtilities-Qt_5_12_3_gcc64-Release/debug/ -lnmfUtilities
else:unix: LIBS += -L$$PWD/../build-nmfUtilities-Qt_5_12_3_gcc64-Release/ -lnmfUtilities

INCLUDEPATH += $$PWD/../nmfUtilities
DEPENDPATH += $$PWD/../nmfUtilities

DISTFILES += \
    README.md
