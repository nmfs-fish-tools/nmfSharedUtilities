#-------------------------------------------------
#
# Project created by QtCreator 2018-04-13T18:01:30
#
#-------------------------------------------------

QT       += core gui charts sql datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nmfCharts
TEMPLATE = lib

#PRECOMPILED_HEADER = /home/rklasky/workspaceQtCreator/MSCAA/precompiled_header.h
CONFIG += precompile_header

CONFIG += c++14

DEFINES += NMFCHARTS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_CXXFLAGS += -std=c++0x

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lboost_system -lboost_filesystem

SOURCES += \
    nmfChartBar.cpp \
    nmfChartLine.cpp \
    nmfChartScatter.cpp \
    nmfChartSurface.cpp \
    nmfProgressWidget.cpp \
    nmfChartLineWithScatter.cpp

HEADERS += \
    nmfChartBar.h \
    nmfChartLine.h \
    nmfChartScatter.h \
    nmfChartSurface.h \
    nmfProgressWidget.h \
    nmfChartLineWithScatter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-nmfUtilities-Qt_5_12_3_gcc64-Release/release/ -lnmfUtilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-nmfUtilities-Qt_5_12_3_gcc64-Release/debug/ -lnmfUtilities
else:unix: LIBS += -L$$PWD/../../build-nmfUtilities-Qt_5_12_3_gcc64-Release/ -lnmfUtilities

INCLUDEPATH += $$PWD/../nmfUtilities
DEPENDPATH += $$PWD/../nmfUtilities
