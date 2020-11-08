TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += \
        -lws2_32 \
	-static \
	-static-libgcc \
	-static-libstdc++

SOURCES += \
        main.cpp \
        server.cpp \
        smtpserver.cpp \
        tcpserver.cpp

HEADERS += \
    ch.h \
    server.h \
    smtpserver.h \
    tcpserver.h \
    ../../errcheck.h
