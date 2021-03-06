QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audiosourcedevice.cpp \
    audiosourcefile.cpp \
    compute-mfcc/mfcc.cc \
    consts.cpp \
    graph.cpp \
    log.cpp \
    main.cpp \
    mainwindow.cpp \
    mmmfcc.cpp \
    settingdialog.cpp \
    settings.cpp \
    status.cpp \
    translator.cpp

HEADERS += \
    audiosourcedevice.h \
    audiosourcefile.h \
    compute-mfcc/mfcc.h \
    consts.h \
    graph.h \
    log.h \
    mainwindow.h \
    mmmfcc.h \
    settingdialog.h \
    settings.h \
    status.h \
    translator.h \
    util.h

FORMS += \
    mainwindow.ui \
    settingdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
