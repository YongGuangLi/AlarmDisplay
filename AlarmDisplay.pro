#-------------------------------------------------
#
# Project created by QtCreator 2018-08-20T09:23:33
#
#-------------------------------------------------

QT       += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlarmDisplay
TEMPLATE = app

CONFIG += uitools

include(/home/liyg/log4qt/log4qt.pri)

SOURCES += main.cpp\
        widget.cpp \
    singleapplication.cpp \
    RtdbMessage.pb.cpp \
    RedisHelper.cpp \
    configini.cpp \
    databasehelper.cpp \
    historyquery.cpp \
    alarmdata.cpp \
    realtimealarm.cpp \
    alarmdispconfig.cpp \
    lightindicator.cpp \
    lineedit.cpp \
    filterbay.cpp \
    configurationinterface.cpp \
    switchstatuscheck.cpp \
    remotesignalling.cpp \
    remotemeasuring.cpp \
    switchactiontimecheck.cpp \
    singlebayprotectaction.cpp \
    soetimestampchange.cpp \
    stationtimesyncabnormol.cpp \
    devicecomminterrupt.cpp \
    powersystemabnormal.cpp \
    electricruncheck.cpp \
    auxiliarypowercheck.cpp \
    userconfig.cpp \
    loginwindow.cpp \
    Base.cpp \
    analysisui.cpp \
    RWIniFile.cpp \
    alarmdetail.cpp \
    HorizontalGraph.cpp \
    VerticalGraph.cpp \
    alarmreportparse.cpp \
    DialogGraph.cpp \
    analysispcapfile.cpp \
    pushbutton.cpp \
    remotecontrolprocess.cpp \
    statisticanalysis.cpp \
    remotecontrolwidget.cpp \
    aboutdialog.cpp \
    primaryequipmentabnormal.cpp \
    secondaryequipmentabnormal.cpp \
    floatingwindow.cpp \
    SmoothCurveGenerator1.cpp

HEADERS  += widget.h \
    singleapplication.h \
    stylehelper.h \
    RtdbMessage.pb.h \
    RedisHelper.h \
    configini.h \
    databasehelper.h \
    historyquery.h \
    alarmdata.h \
    realtimealarm.h \
    alarmdispconfig.h \
    lightindicator.h \
    lineedit.h \
    filterbay.h \
    configurationinterface.h \
    switchstatuscheck.h \
    remotesignalling.h \
    remotemeasuring.h \
    switchactiontimecheck.h \
    singlebayprotectaction.h \
    soetimestampchange.h \
    stationtimesyncabnormol.h \
    devicecomminterrupt.h \
    powersystemabnormal.h \
    electricruncheck.h \
    auxiliarypowercheck.h \
    userconfig.h \
    loginwindow.h \
    Base.h \
    analysisui.h \
    IncludeFile.h \
    StructComm.h \
    RWIniFile.h \
    alarmdetail.h \
    DataSturct.h \
    HorizontalGraph.h \
    VerticalGraph.h \
    alarmreportparse.h \
    DialogGraph.h \
    analysispcapfile.h \
    StructClass.h \
    pushbutton.h \
    remotecontrolprocess.h \
    statisticanalysis.h \
    remotecontrolwidget.h \
    aboutdialog.h \
    primaryequipmentabnormal.h \
    secondaryequipmentabnormal.h \
    floatingwindow.h \
    SmoothCurveGenerator1.h

FORMS    += widget.ui \
    historyquery.ui \
    realtimealarm.ui \
    alarmdispconfig.ui \
    lightindicator.ui \
    filterbay.ui \
    configurationinterface.ui \
    switchstatuscheck.ui \
    remotesignalling.ui \
    remotemeasuring.ui \
    switchactiontimecheck.ui \
    singlebayprotectaction.ui \
    soetimestampchange.ui \
    stationtimesyncabnormol.ui \
    devicecomminterrupt.ui \
    powersystemabnormal.ui \
    electricruncheck.ui \
    auxiliarypowercheck.ui \
    userconfig.ui \
    loginwindow.ui \
    alarmdetail.ui \
    analysispcapfile.ui \
    remotecontrolprocess.ui \
    statisticanalysis.ui \
    remotecontrolwidget.ui \
    aboutdialog.ui \
    primaryequipmentabnormal.ui \
    secondaryequipmentabnormal.ui \
    floatingwindow.ui

RESOURCES += \
    alarmdisplay.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/protobuf-3.3.0/lib/release/ -lprotobuf-lite
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/protobuf-3.3.0/lib/debug/ -lprotobuf-lite
else:unix: LIBS += -L$$PWD/protobuf-3.3.0/lib/ -lprotobuf-lite

INCLUDEPATH += $$PWD/protobuf-3.3.0/include
DEPENDPATH += $$PWD/protobuf-3.3.0/include

INCLUDEPATH += $$PWD/Control

INCLUDEPATH += /home/liyg/wireshark-1.8.10
INCLUDEPATH += /home/liyg/glib/include/glib-2.0
INCLUDEPATH += /home/liyg/glib/lib/glib-2.0/include

LIBS += -L/home/liyg/glib/lib -lglib-2.0
LIBS += -L/usr/local/lib -lpcap
LIBS += -L/usr/local/wireshark-1.8.10/lib -lwireshark


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libacl/lib/release/ -lacl_cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libacl/lib/debug/ -lacl_cpp
else:unix: LIBS += -L$$PWD/libacl/lib/ -lacl_cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libacl/lib/release/ -lacl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libacl/lib/debug/ -lacl
else:unix: LIBS += -L$$PWD/libacl/lib/ -lacl


INCLUDEPATH += $$PWD/libacl/include
DEPENDPATH += $$PWD/libacl/include

