#-------------------------------------------------
#
# Project created by QtCreator 2020-03-06T23:09:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CatCircus
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        src/game/game.cpp \
    src/game/game_objects/timebar.cpp \
        src/game/gamewidget.cpp \
    src/game/graphicsobject.cpp \
    src/game/minigame.cpp \
    src/game/minigames/testminigame.cpp \
    src/game/pointspage.cpp \
        src/menu/aboutwidget.cpp \
        src/menu/main.cpp \
        src/menu/mainwindow.cpp \
        src/menu/settingswidget.cpp \

HEADERS += \
        src/game/game.h \
        src/game/game_objects/timebar.h \
        src/game/gamewidget.h \
        src/game/graphicsobject.h \
        src/game/minigame.h \
        src/game/minigames/testminigame.h \
        src/game/pointspage.h \
        src/menu/aboutwidget.h \
        src/menu/mainwindow.h \
        src/menu/settingswidget.h

FORMS += \
        src/game/gamewidget.ui \
        src/game/pointspage.ui \
        src/menu/aboutwidget.ui \
        src/menu/mainwindow.ui \
        src/menu/settingswidget.ui

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
