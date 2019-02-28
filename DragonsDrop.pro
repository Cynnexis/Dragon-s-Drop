#-------------------------------------------------
#
# Project created by QtCreator 2019-01-30T13:06:19
#
#-------------------------------------------------

QT       += core gui svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DragonsDrop
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
        main.cpp \
        mainwindow.cpp \
    p.cpp \
    r.cpp \
    clip.cpp \
    mapmodel.cpp \
    editclipboard.cpp \
    edittextclipboard.cpp \
    editimageclipboard.cpp \
    editcolorclipboard.cpp

HEADERS += \
        mainwindow.h \
    p.h \
    r.h \
    clip.h \
    mapmodel.h \
    editclipboard.h \
    edittextclipboard.h \
    editimageclipboard.h \
    editcolorclipboard.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    doxy.conf \
    README.md \
    LICENSE.txt \
    res/README.html

RESOURCES += \
    dragonsdrop.qrc
