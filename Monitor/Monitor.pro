QT       += core gui
QT       += network
QT       += serialport
QT       += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/eigen-3.4.0

include ($$PWD/Communication/Communication.pri)
include ($$PWD/DataBase/DataBase.pri)
include ($$PWD/Model/Model.pri)
include ($$PWD/LogManagement/LogManagement.pri)
include ($$PWD/ConfigManagement/ConfigManagement.pri)
include ($$PWD/UserManagement/UserManagement.pri)

SOURCES += \
    catalogwidget.cpp \
    catalogwidget2.cpp \
    global.cpp \
    iconwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    catalogwidget.h \
    catalogwidget2.h \
    datastruct.h \
    global.h \
    iconwidget.h \
    mainwindow.h

FORMS += \
    catalogwidget.ui \
    catalogwidget2.ui \
    iconwidget.ui \
    mainwindow.ui

RESOURCES += \
    UISource.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
