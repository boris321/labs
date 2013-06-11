#-------------------------------------------------
#
# Project created by QtCreator 2012-05-31T02:58:02
#
#-------------------------------------------------

QT       += core gui

TARGET = enc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    md5.cpp \
    functions.cpp \
    des.cpp \
    sha1.cpp \
    gost.cpp \
    gosthash.cpp \
    aes.cpp \
    idea.cpp \
    ripemd.cpp \
    encription.cpp

HEADERS  += mainwindow.h \
    md5.h \
    des.h \
    sha1.h \
    gost.h \
    gosthash.h \
    aes.h \
    ripemd.h

FORMS    += mainwindow.ui
