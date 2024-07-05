QT       += core gui
QT       += printsupport

HEADERS += \
    $$PWD/commontablemodel.h \
    $$PWD/itemdelegate.h \
    $$PWD/messagetips.h \
    $$PWD/multicurvesplot.h \
    $$PWD/qcustomplot.h \
    $$PWD/splashscreen.h

SOURCES += \
    $$PWD/commontablemodel.cpp \
    $$PWD/itemdelegate.cpp \
    $$PWD/messagetips.cpp \
    $$PWD/multicurvesplot.cpp \
    $$PWD/qcustomplot.cpp \
    $$PWD/splashscreen.cpp

FORMS += \
    $$PWD/splashscreen.ui

include ($$PWD/IconHelper/IconHelper.pri)
include ($$PWD/MyButton/MyButton.pri)
include ($$PWD/MyCheckBox/MyCheckBox.pri)
include ($$PWD/MyTab/MyTab.pri)

