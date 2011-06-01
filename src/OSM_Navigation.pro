SOURCES = main.cpp \
    slippymap.cpp \
    lightmaps.cpp \
    mapzoom.cpp \
    tracks.cpp \
    navigation.cpp
QT += network
TEMPLATE = app
CONFIG += designer
HEADERS += slippymap.h \
    lightmaps.h \
    mapzoom.h \
    tracks.h \
    navigation.h
OTHER_FILES += 
TRANSLATIONS += OSM_Nav_sk.ts
QMAKE_CFLAGS_ += -ggdb -g3
