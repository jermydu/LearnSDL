TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c

#生成exe 目录
DESTDIR = $$PWD/../Bin

#include
INCLUDEPATH += $$PWD/../SDL2-2.28.5/include
#lib
LIBS +=   $$PWD/../SDL2-2.28.5/lib/x64/SDL2.lib
