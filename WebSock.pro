#-------------------------------------------------
#
# Project created by QtCreator 2016-07-14T20:44:37
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = WebSock
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS    +=-levent

CONFIG += c++11

SOURCES += main.cpp \
    baseserver.cpp \
    tools.cpp

HEADERS += \
    baseserver.h \
    util.h \
    structure.h \
    tools.h
