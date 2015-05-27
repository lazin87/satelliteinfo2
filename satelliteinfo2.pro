TEMPLATE = app

QT += qml quick positioning
CONFIG += c++11
SOURCES += main.cpp \
    satelliteinfo.cpp \
    cgpspositionprovider.cpp \
    idatastorage.cpp \
    cgpsdatastorage.cpp \
    idata.cpp \
    cgpspositiondata.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    satelliteinfo.h \
    cgpspositionprovider.h \
    idatastorage.h \
    cgpsdatastorage.h \
    idata.h \
    cgpspositiondata.h
