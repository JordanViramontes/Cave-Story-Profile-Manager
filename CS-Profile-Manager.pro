QT       += core gui
# CONFIG += static
# CONFIG += staticlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# CONFIG+=c++11

# You can make your code fail to compile if it uses deprecated APIs.
# Uncomment the following line to disable deprecated APIs.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # Disables all APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    profileloader.cpp \
    tablewidgetdragrows.cpp \
    weaponwidget.cpp \
    window.cpp

HEADERS += \
    global.h \
    mainwindow.h \
    profileloader.h \
    tablewidgetdragrows.h \
    window.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
