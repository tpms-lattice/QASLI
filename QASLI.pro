#-------------------------------------------------
#
# Project created by QtCreator 2012-08-26T14:02:01
#
#-------------------------------------------------

QT       += core gui xml 3dcore 3drender 3dinput 3dextras widgets

VERSION = 0.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QASLI
TEMPLATE = app

CONFIG -= debug_and_release debug_and_release_target

SOURCES += src/main.cpp\
    src/about.cpp \
    src/FileSystemPathUtils.cpp \
        src/mainwindow.cpp\

HEADERS  += src/mainwindow.h\
    src/about.h \
    src/FileSystemPathUtils.h

FORMS    += mainwindow.ui \
    about.ui

INCLUDEPATH += $$PWD/src

#LIBS += -L../ASLI/build/ -lASLI -lalg -lyaml -ltet -lgmp -lmpfr -lstdc++fs
#LIBS += -L ~/meshing/intel_tbb/tbb/lib/intel64/gcc4.8/ -ltbb -lpthread -ltbbmalloc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib
else:unix: LIBS += -L/usr/lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib
else:unix: LIBS += -L$$PWD/lib
