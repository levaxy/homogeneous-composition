QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        AddStZn.cpp \
        Averaging.cpp \
        Cell.cpp \
        Container.cpp \
        FromQJArray_toVector.cpp \
        Granulating.cpp \
        Initialisation.cpp \
        Laba.cpp \
        Layer.cpp \
        Manipulator.cpp \
        NullContainer.cpp \
        Operation.cpp \
        Press.cpp \
        Probe.cpp \
        Storage.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    AddStZn.h \
    Averaging.h \
    Cell.h \
    Container.h \
    Granulating.h \
    IncObj.h \
    IncludeJson.h \
    Initialisation.h \
    Laba.h \
    Layer.h \
    Manipulator.h \
    NullContainer.h \
    Operation.h \
    Press.h \
    Probe.h \
    Storage.h
