#-------------------------------------------------
#
# Project created by QtCreator 2025-07-30T16:13:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = album
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

INCLUDEPATH += $$PWD/inc

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
    src/wizard.cpp \
    src/prosetpage.cpp \
    src/confirmpage.cpp \
    src/protree.cpp \
    src/protreewidget.cpp \
    src/protreeitem.cpp \
    src/protreethread.cpp \
    src/removeprodialog.cpp \
    src/opentreethread.cpp

HEADERS += \
        inc/mainwindow.h \
    inc/wizard.h \
    inc/prosetpage.h \
    inc/confirmpage.h \
    inc/protree.h \
    inc/protreewidget.h \
    inc/protreeitem.h \
    inc/const.h \
    inc/protreethread.h \
    inc/removeprodialog.h \
    inc/opentreethread.h

FORMS += \
        ui/mainwindow.ui \
    ui/wizard.ui \
    ui/prosetpage.ui \
    ui/confirmpage.ui \
    ui/protree.ui \
    ui/removeprodialog.ui

RESOURCES += \
    rc.qrc

win32{
    RC_ICONS = "icon/bulm.ico"
}
