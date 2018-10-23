#-------------------------------------------------
#
# Project created by QtCreator 2018-07-04T10:05:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = usb_storage_check
TEMPLATE = app
#LIBS += -L"C:/OpenSSL-Win32/bin" -llibeay32
#LIBS += -L"C:/OpenSSL-Win32/bin" -lssleay32
#INCLUDEPATH += $$quote(C:/OpenSSL-Win32/include)

LIBS += -L"OpenSSL-Win32/bin" -llibeay32
LIBS += -L"OpenSSL-Win32/bin" -lssleay32
INCLUDEPATH += $$quote(OpenSSL-Win32/include)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

