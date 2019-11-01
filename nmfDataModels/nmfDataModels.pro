#-------------------------------------------------
#
# Project created by QtCreator 2017-03-04T00:59:50
#
#-------------------------------------------------

QT       -= gui
QT       += widgets sql

CONFIG += c++14

TARGET = nmfDataModels
TEMPLATE = lib

DEFINES += NMFDATAMODELS_LIBRARY

SOURCES += \
    nmfEntityModel.cpp \
    nmfInitialSelectivityModel.cpp \
    nmfMaturityModel.cpp \
    nmfMortalityModel.cpp \
    nmfData.cpp \
    nmfVonBertModel.cpp
#    nmfVarFishMortModel.cpp

HEADERS +=\
    nmfEntityModel.h \
    nmfInitialSelectivityModel.h \
    nmfMaturityModel.h \
    nmfMortalityModel.h \
    nmfData.h \
    nmfVonBertModel.h
#    nmfvarfishmortmodel_global.h \
#    nmfVarFishMortModel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-nmfDatabase-Qt_5_12_3_gcc64-Release/release/ -lnmfDatabase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-nmfDatabase-Qt_5_12_3_gcc64-Release/debug/ -lnmfDatabase
else:unix: LIBS += -L$$PWD/../../build-nmfDatabase-Qt_5_12_3_gcc64-Release/ -lnmfDatabase

INCLUDEPATH += $$PWD/../../nmfSharedUtilities/nmfDatabase
DEPENDPATH += $$PWD/../../nmfSharedUtilities/nmfDatabase

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-nmfUtilities-Qt_5_12_3_gcc64-Release/release/ -lnmfUtilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-nmfUtilities-Qt_5_12_3_gcc64-Release/debug/ -lnmfUtilities
else:unix: LIBS += -L$$PWD/../../build-nmfUtilities-Qt_5_12_3_gcc64-Release/ -lnmfUtilities

INCLUDEPATH += $$PWD/../../nmfSharedUtilities/nmfUtilities
DEPENDPATH += $$PWD/../../nmfSharedUtilities/nmfUtilities

