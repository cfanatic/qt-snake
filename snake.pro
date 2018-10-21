QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake
TEMPLATE = app

SOURCES += src/main.cpp \
           src/snake.cpp

HEADERS += src/snake.h

FORMS += snake.ui

ICON = res/snake.icns
