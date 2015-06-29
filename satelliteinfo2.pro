TEMPLATE = app

QT += qml quick positioning sql
CONFIG += c++11
SOURCES += main.cpp \
    satelliteinfo.cpp \
    cgpspositionprovider.cpp \
    cgpsdatastorage.cpp \
    idata.cpp \
    cgpspositiondata.cpp \
    tests.cpp \
    iremotedatastorage.cpp \
    ilocaldatastorage.cpp \
    csqldatastorage.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    satelliteinfo.h \
    cgpspositionprovider.h \
    cgpsdatastorage.h \
    idata.h \
    cgpspositiondata.h \
    iremotedatastorage.h \
    ilocaldatastorage.h \
    csqldatastorage.h
