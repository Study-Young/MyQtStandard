SOURCES += \
    $$PWD/qhttprequest.cpp \
    $$PWD/qhttpresponse.cpp \
    $$PWD/qhttpserver.cpp \
    $$PWD/http_parser.c \
    $$PWD/threadhandle.cpp \
    $$PWD/qasyntcpserver.cpp \
    $$PWD/qasynhttpsocket.cpp \
    $$PWD/staticfilecontroller.cpp \
    $$PWD/httphandler.cpp \
    $$PWD/qasyntcpsocket.cpp

HEADERS  += \
    $$PWD/http_parser.h \
    $$PWD/qhttprequest.h \
    $$PWD/qhttpresponse.h \
    $$PWD/threadhandle.h \
    $$PWD/qasyntcpserver.h \
    $$PWD/qasynhttpsocket.h \
    $$PWD/qhttpserver.h \
    $$PWD/staticfilecontroller.h \
    $$PWD/httphandler.h \
    $$PWD/qasyntcpsocket.h \
    $$PWD/qhttpserverfwd.h

unix:include($$PWD/eventdispatcher_libev/eventdispatcher_libev.pri)
