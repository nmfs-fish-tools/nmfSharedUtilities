#-------------------------------------------------
#
# Project created by QtCreator 2019-08-07T12:08:14
#
#-------------------------------------------------

QT       += core gui charts sql datavisualization uitools

TARGET = nmfDynamics
TEMPLATE = lib

CONFIG += c++14

DEFINES += NMFDYNAMICS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lboost_system -lboost_filesystem

SOURCES += \
    nmfAbundance.cpp

HEADERS += \
    nmfAbundance.h

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


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MSCAA_GuiUtilities-Qt_5_12_3_gcc64-Release/release/ -lMSCAA_GuiUtilities
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MSCAA_GuiUtilities-Qt_5_12_3_gcc64-Release/debug/ -lMSCAA_GuiUtilities
#else:unix: LIBS += -L$$PWD/../build-MSCAA_GuiUtilities-Qt_5_12_3_gcc64-Release/ -lMSCAA_GuiUtilities

#INCLUDEPATH += $$PWD/../MSCAA_GuiUtilities
#DEPENDPATH += $$PWD/../MSCAA_GuiUtilities
