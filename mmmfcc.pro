QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    abstractaudiosource.cpp \
    audiosourcedevice.cpp \
    audiosourcefile.cpp \
    compute-mfcc/mfcc.cc \
    compute-mfcc/settings.cpp \
    graph.cpp \
    log.cpp \
    main.cpp \
    mainwindow.cpp \
    mmmfcc.cpp \
    translator.cpp

HEADERS += \
    abstractaudiosource.h \
    audiosourcedevice.h \
    audiosourcefile.h \
    compute-mfcc/mfcc.h \
    compute-mfcc/wavHeader.h \
    graph.h \
    log.h \
    mainwindow.h \
    mmmfcc.h \
    settings.h \
    translator.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
