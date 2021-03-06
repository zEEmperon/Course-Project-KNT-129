QT       += core gui  sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addeventdialog.cpp \
    addhomework.cpp \
    dbmanager.cpp \
    errordialog.cpp \
    event.cpp \
    exception.cpp \
    infodialog.cpp \
    main.cpp \
    mainwindow.cpp \
    notificationdialog.cpp \
    schedule.cpp

HEADERS += \
    addeventdialog.h \
    addhomework.h \
    db_data.h \
    dbmanager.h \
    errordialog.h \
    event.h \
    eventsEnum.h \
    exception.h \
    infodialog.h \
    mainwindow.h \
    notificationdialog.h \
    schedule.h

FORMS += \
    addeventdialog.ui \
    addhomework.ui \
    errordialog.ui \
    infodialog.ui \
    mainwindow.ui \
    notificationdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
