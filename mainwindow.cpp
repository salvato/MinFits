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
#include <QMessageBox>

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
//    double vk0, vg0, vk, vg;
//    vk = 0.0;
//    vg = 0.0;
//    krab(0.0, M_PI, cos, &vk, &vg);
//    vk0 = vk;
//    vg0 = vg;
//    std::cout << "integrale(cos(x)) in [0   M_PI] vk=" << vk0 << " vg=" << vg0 << endl;
//    krab(M_PI, 2.0*M_PI, cos, &vk, &vg);
//    vk0 += vk;
//    vg0 += vg;
//    std::cout << "integrale(cos(x)) in [0 2*M_PI] vk=" << vk0 << " vg=" << vg0 << endl;
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


bool
MainWindow::getSummCosInitialValues() {
    if(!readParamFile("Open SUMM-COS Param File", "Summ Cos Param files (*.cmd *.par)"))
        return false;
   return true;
}


bool
MainWindow::getSummSinInitialValues() {
    if(!readParamFile("Open SUMM-Sin Param File", "Summ Sin Param files (*.cmd *.par)"))
        return false;
    return true;
}


bool
MainWindow::readParamFile(QString sWindowTitle, QString sFileFilter) {
    QFileDialog chooseFileDialog(nullptr,
                                 sWindowTitle,
                                 sDataDir,
                                 sFileFilter);
    chooseFileDialog.setFileMode(QFileDialog::ExistingFile);
    if(!chooseFileDialog.exec())
        return false;

    QString sFilename = chooseFileDialog.selectedFiles().at(0);
    if(sFilename == QString())
        return false;

    sDataDir = chooseFileDialog.directory().absolutePath();
    settings.setValue("Data_Dir", sDataDir);
    QFile dataFile(sFilename);
    if(!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      return false;
    }
    QString line;
    QStringList lineElems;
    bool ok;
    try {
        while(!dataFile.atEnd()) {
            line = dataFile.readLine(100);
            lineElems = line.split(",");
            if(lineElems.size() < 3) {
                QMessageBox::critical(this,
                                      "Error",
                                      QString("Malformed Parameter %1").arg(pParams->getParams().Params().size())
                                      );
                dataFile.close();
                return false;
            }
            QString sName   = lineElems.at(0);
            double value    = lineElems.at(1).toDouble(&ok);
            double error    = lineElems.at(2).toDouble(&ok);
            double minValue = 0.0;
            double maxValue = 0.0;
            if(lineElems.size() > 4) {
                minValue = lineElems.at(3).toDouble(&ok);
                maxValue = lineElems.at(4).toDouble(&ok);
            }
            bool bFixed = false;
            if(lineElems.size() > 5)
                bFixed = lineElems.at(5).contains(QString("fixed"));

            pParams->Add(sName, value, error, minValue, maxValue, bFixed);
        }
        dataFile.close();
    } catch(...) {
        QMessageBox::critical(this,
                              "Error",
                              QString("Malformed File %1").arg(sFilename)
                              );
        dataFile.close();
        return false;
    }

    dataFile.close();
    return true;
}


void
MainWindow::on_SummCos_clicked() {
    if(pFunctionToMinimize) delete pFunctionToMinimize;
    pFunctionToMinimize = new SummCosFunction();

    if(pParams) delete pParams;
    pParams = new ParametersWindow(pFunctionToMinimize, "Summ + Cos Fit Parameters", nullptr);

    if(!getSummCosInitialValues())
        return;

    connect(pParams, SIGNAL(closing()),
            this, SLOT(onFitDone()));
    hide();
    pParams->show();
}


void
MainWindow::on_SummSin_clicked() {
    if(pFunctionToMinimize) delete pFunctionToMinimize;
    pFunctionToMinimize = new SummSinFunction();

    if(pParams) delete pParams;
    pParams = new ParametersWindow(pFunctionToMinimize, "Summ + Sin Fit Parameters", nullptr);

    if(!getSummSinInitialValues())
        return;

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
}

