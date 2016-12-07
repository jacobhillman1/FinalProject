TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    commentmetric.cpp \
    couplingmetric.cpp \
    directoryparser.cpp \
    dsstring.cpp \
    file.cpp \
    indentationmetric.cpp \
    metricinfo.cpp \
    tests.cpp \
    variable.cpp \
    variablemetric.cpp

SUBDIRS += \
    FinalProject.pro

DISTFILES += \
    FinalFinalProject.pro.user \
    FinalProject.pro.user

HEADERS += \
    abstractmetric.h \
    avltree.h \
    catch.hpp \
    commentmetric.h \
    couplingmetric.h \
    directoryparser.h \
    dsstring.h \
    dsvector.h \
    file.h \
    hashtable.h \
    indentationmetric.h \
    linkedlist.h \
    metricinfo.h \
    variable.h \
    variablemetric.h

