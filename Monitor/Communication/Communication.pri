HEADERS += \
    $$PWD/datahandlemodule.h \
    $$PWD/httpmodule.h \
    $$PWD/modbusmodule.h \
    $$PWD/myhttpmodule.h \
    $$PWD/tcpmodule.h \
    $$PWD/udpmodule.h


SOURCES += \
    $$PWD/datahandlemodule.cpp \
    $$PWD/httpmodule.cpp \
    $$PWD/modbusmodule.cpp \
    $$PWD/myhttpmodule.cpp \
    $$PWD/tcpmodule.cpp \
    $$PWD/udpmodule.cpp

include ($$PWD/modbus/modbus.pri)
include ($$PWD/HttpServer/httpserver.pri)
