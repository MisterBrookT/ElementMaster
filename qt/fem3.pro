QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#RC_ICONS = tubiao.png
SOURCES += \
    MyDrawingWidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    MyDrawingWidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32: LIBS += -L$$PWD/../../gmsh_4.11.1-Windows64-sdk/lib/ -lgmsh

INCLUDEPATH += $$PWD/../../gmsh_4.11.1-Windows64-sdk/include
INCLUDEPATH += D:/eigen-3.4.0
DEPENDPATH += $$PWD/../../gmsh_4.11.1-Windows64-sdk/include

RESOURCES += \
    logo.qrc

DISTFILES += \
    logo.rc

RC_FILE = logo.rc

