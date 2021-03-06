QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


unix:    INCLUDEPATH += /usr/local/include/Minuit2
windows: INCLUDEPATH += "C:\Users\gabriele\Documents\qtprojects\Minuit2\inc"

SOURCES += \
    AxisFrame.cpp \
    AxisLimits.cpp \
    DataSetProperties.cpp \
    axesdialog.cpp \
    datastream2d.cpp \
    dceul.cpp \
    gammln.cpp \
    krab.cpp \
    main.cpp \
    mainwindow.cpp \
    msgwindow.cpp \
    parameterline.cpp \
    parameterswindow.cpp \
    plot2d.cpp \
    plotpropertiesdlg.cpp \
    summcosfunction.cpp \
    summsinfunction.cpp \
    twogaussfunction.cpp

HEADERS += \
    AxisFrame.h \
    AxisLimits.h \
    DataSetProperties.h \
    MinimizationFunction.h \
    axesdialog.h \
    datastream2d.h \
    dceul.h \
    gammln.h \
    krab.h \
    mainwindow.h \
    msgwindow.h \
    parameterline.h \
    parameterswindow.h \
    plot2d.h \
    plotpropertiesdlg.h \
    summcosfunction.h \
    summsinfunction.h \
    twogaussfunction.h

unix: LIBS += \
    -L"/usr/local/lib" \
    -lMinuit2 \# To include libMinuit2.a         from /usr/local/lib
    -lMinuit2Math \# To include libMinuit2Math.a from /usr/local/lib

windows: {
    Release: LIBS += \
        -L"C:\Users\gabriele\Documents\qtprojects\Minuit2\build\Release\lib" \
        -lMinuit2 \# To include libMinuit2.a         from /usr/local/lib
        -lMinuit2Math \# To include libMinuit2Math.a from /usr/local/lib

    Debug: LIBS += \
        -L"C:\Users\gabriele\Documents\qtprojects\Minuit2\build\Debug\lib" \
        -lMinuit2 \# To include libMinuit2.a         from /usr/local/lib
        -lMinuit2Math \# To include libMinuit2Math.a from /usr/local/lib
}

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    DatiTest/PVB50_1Hz_absorption.dat \
    DatiTest/PVB80new_alpha1_2_1Hz.dat \
    DatiTest/PVB80new_alpha1_2_1Hz.par \
    DatiTest/PVB_1Hz.dat \
    DatiTest/summCos.par \
    DatiTest/summSin.par \
    plot.png

RESOURCES += \
    Resources.qrc
