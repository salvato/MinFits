#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "summcosfunction.h"
#include "summsinfunction.h"

#include <iostream>
#include <fstream>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <math.h>

std::vector<double> theFit;

using namespace ROOT;
using namespace Minuit2;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , pUi(new Ui::MainWindow)
    , pOut(nullptr)
    , pParams(nullptr)
    , pMsgWindow(nullptr)
    , pFunctionToMinimize(nullptr)
{
    pUi->setupUi(this);
    getSettings();
    pMsgWindow = new MsgWindow();
    pMsgWindow->show();
    pOut = new QDebugStream(std::cout, &pMsgWindow->textEdit);
}


MainWindow::~MainWindow() {
    if(pUi) delete pUi;
    if(pOut) delete pOut;
    if(pParams) delete pParams;
    if(pMsgWindow) delete pMsgWindow;
    if(pFunctionToMinimize) delete pFunctionToMinimize;
}


void
MainWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event)
    saveSettings();
    if(pUi) delete pUi;
    pUi = nullptr;
    if(pOut) delete pOut;
    pOut = nullptr;
    if(pParams) delete pParams;
    pParams = nullptr;
    if(pMsgWindow) delete pMsgWindow;
    pMsgWindow = nullptr;
    if(pFunctionToMinimize) delete pFunctionToMinimize;
    pFunctionToMinimize = nullptr;
}


void
MainWindow::getSettings() {
    restoreGeometry(settings.value(QString("MainWindow_Dialog")).toByteArray());
    sDataDir = settings.value("Data_Dir",   QDir::currentPath()).toString();
}


void
MainWindow::saveSettings() {
    settings.setValue(QString("MainWindow_Dialog"), saveGeometry());
    settings.setValue("Data_Dir", sDataDir);
}


void
MainWindow::on_SummCos_clicked() {
    if(pParams) delete pParams;
    if(pFunctionToMinimize) delete pFunctionToMinimize;
    pFunctionToMinimize = new SummCosFunction();
    pParams = new ParametersWindow(pFunctionToMinimize, "Summ + Cos Fit Parameters", nullptr);

    pParams->Add("BETA1",       1.108, 0.20880,   0.2,   5.00, false);
    pParams->Add("BETA2",      10.520, 0.13150,   0.45,  0.58, false);
    pParams->Add("TM(+3)",      2.172, 0.11327,   1.0,   7.00, false);
    pParams->Add("TAU(-14)",    3.100, 0.90082,   0.1,  20.00, false);
    pParams->Add("COST1(-3)",   9.446, 2.00700,   0.1,  20.00, false);
    pParams->Add("T00",       258.300, 4.28760, 100.0, 400.00, false);

    connect(pParams, SIGNAL(closing()),
            this, SLOT(onFitDone()));
    hide();
    pParams->show();
}


void
MainWindow::onFitDone() {
    if(pParams) delete pParams;
    pParams = nullptr;
    if(pFunctionToMinimize) delete pFunctionToMinimize;
    pFunctionToMinimize = nullptr;
    show();
/*
    if(pParams->getParams().)
    settings.setValue(QString("MainWindow_Dialog"), saveGeometry());
    settings.setValue("Data_Dir", sDataDir);
*/
}


void
MainWindow::on_SummSin_clicked() {
    if(pParams) delete pParams;
    if(pFunctionToMinimize) delete pFunctionToMinimize;
    pFunctionToMinimize = new SummSinFunction();
    pParams = new ParametersWindow(pFunctionToMinimize, "Summ + Sin Fit Parameters", nullptr);

    pParams->Add("BETA1",       0.250, 0.20880,   0.2,   5.00, false);
    pParams->Add("BETA2",       0.290, 0.13150,   0.1,   0.58, false);
    pParams->Add("TM(+3)",      2.020, 0.11327,   1.0,   7.00, false);
    pParams->Add("TAU(-14)",    3.680, 0.90082,   0.1,  20.00, false);
    pParams->Add("T00",       119.000, 4.28760, 100.0, 400.00, false);

    connect(pParams, SIGNAL(closing()),
            this, SLOT(onFitDone()));
    hide();
    pParams->show();
}
