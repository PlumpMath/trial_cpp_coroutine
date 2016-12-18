TEMPLATE = app

QT       += core
QT       += gui widgets

CONFIG += c++11 silent

SOURCES += main.cpp invoker.cpp

HEADERS += invoker.h

MOC_DIR = ./Intermediate/moc
OBJECTS_DIR = ./Intermediate/obj

QMAKE_CXXFLAGS += -Wall -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-missing-field-initializers
