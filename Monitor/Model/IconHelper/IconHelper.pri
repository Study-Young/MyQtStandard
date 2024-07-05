QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

HEADERS += \
    $$PWD/iconhelper.h \
    $$PWD/panelwidget.h

SOURCES += \
    $$PWD/iconhelper.cpp \
    $$PWD/panelwidget.cpp

RESOURCES += \
    $$PWD/IconHelper.qrc
