TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    app.cpp \
    cagelab_js.cpp
SOURCES += trimesh/load_save_trimesh.cpp
SOURCES += coordinates/meanvalue.cpp
SOURCES += coordinates/green.cpp
SOURCES += cage/cage.cpp


HEADERS += coordinates/BarycentricCoordinates.h \
    app.h
HEADERS += coordinates/meanvalue.h
HEADERS += coordinates/green.h
HEADERS += trimesh/trimesh.h
HEADERS += cage/cage.h
