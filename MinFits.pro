QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    /usr/local/include/Minuit2

SOURCES += \
    AxisFrame.cpp \
    AxisLimits.cpp \
    DataSetProperties.cpp \
    axesdialog.cpp \
    datastream2d.cpp \
    dceul.cpp \
    gammln.cpp \
    main.cpp \
    mainwindow.cpp \
    msgwindow.cpp \
    parameterline.cpp \
    parameterswindow.cpp \
    plot2d.cpp \
    plotpropertiesdlg.cpp \
    qdebugstream.cpp \
    summcosfunction.cpp \
    summsinfunction.cpp \
    twogaussfunction.cpp

HEADERS += \
    AxisFrame.h \
    AxisLimits.h \
    DataSetProperties.h \
    axesdialog.h \
    datastream2d.h \
    dceul.h \
    gammln.h \
    mainwindow.h \
    msgwindow.h \
    parameterline.h \
    parameterswindow.h \
    plot2d.h \
    plotpropertiesdlg.h \
    qdebugstream.h \
    summcosfunction.h \
    summsinfunction.h \
    twogaussfunction.h

LIBS += \
    -L"/usr/local/lib" \
    -lMinuit2 \# To include libMinuit2.a         from /usr/local/lib
    -lMinuit2Math \# To include libMinuit2Math.a from /usr/local/lib

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    DatiTest/PVB-1Hz alfa param.cmd \
    DatiTest/PVB-1Hz alfa param.cmd \
    DatiTest/PVB-1Hz alfa param.cmd \
    DatiTest/PVB-1Hz_alfa_param.cmd \
    DatiTest/PVB_1Hz.dat
