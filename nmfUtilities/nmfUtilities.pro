#-------------------------------------------------
#
# Project created by QtCreator 2017-03-03T23:43:10
#
#-------------------------------------------------

#QT       += gui
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = nmfUtilities
TEMPLATE = lib

DEFINES += NMFUTILITIES_LIBRARY

CONFIG += c++14

LIBS += -lboost_system -lboost_filesystem

SOURCES += \
    nmfUtils.cpp \
    nmfLogger.cpp \
    nmfUtilsStatistics.cpp \
    nmfUtilsQt.cpp \
    nmfUtilsComplex.cpp \
    nmfUtilsSolvers.cpp \
    nmfRandom.cpp

HEADERS +=\
    nmfConstantsMSVPA.h \
    nmfUtils.h \
    nmfConstants.h \
    nmfLogger.h \
    nmfUtilsStatistics.h \
    nmfUtilsQt.h \
    nmfUtilsComplex.h \
    nmfUtilsSolvers.h \
    nmfStructsQt.h \
    nmfConstantsMSSPM.h \
    nmfRandom.h \
    nmfStructsBees.h \
    nmfConstantsMSCAA.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


