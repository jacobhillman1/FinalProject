TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    dsstring.cpp \
    tests.cpp \
    directoryparser.cpp \
    commentmetric.cpp \
    indentationmetric.cpp \
    file.cpp \
    variable.cpp \
    metricinfo.cpp \
    variablemetric.cpp \
    couplingmetric.cpp

HEADERS += \
    dsstring.h \
    hashtable.h \
    dsvector.h \
    linkedlist.h \
    catch.hpp \
    abstractmetric.h \
    directoryparser.h \
    commentmetric.h \
    indentationmetric.h \
    file.h \
    avltree.h \
    variable.h \
    metricinfo.h \
    variablemetric.h \
    couplingmetric.h

