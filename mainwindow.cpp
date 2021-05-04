/*
 *
Copyright (C) 2021  Gabriele Salvato

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/
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
#include "krab.h"

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
    double vk0, vg0, vk, vg;
    vk = 0.0;
    vg = 0.0;
    krab(0.0, M_PI, sin, &vk, &vg);
    vk0 = vk;
    vg0 = vg;
    krab(M_PI, 2.0*M_PI, sin, &vk, &vg);
    vk0 += vk;
    vg0 += vg;
    std::cout << "vk=" << vk0 << " vg=" << vg0;
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
