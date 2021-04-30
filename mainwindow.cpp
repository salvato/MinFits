#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plot2d.h"
#include "summcosfunction.h"

#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnUserParameters.h"
#include "Minuit2/MnPrint.h"

#include <iostream>
#include <fstream>
#include <QFileDialog>
#include <math.h>

using namespace ROOT;
using namespace Minuit2;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , pConsole(nullptr)
    , ui(new Ui::MainWindow)
    , pOut(nullptr)
    , pPlotA(nullptr)
    , pPlotV(nullptr)
{
    ui->setupUi(this);
    pConsole = new QTextEdit();
    pConsole->document()->setMaximumBlockCount(10000);
    pConsole->show();
    pOut = new QDebugStream(std::cout, pConsole);
}


MainWindow::~MainWindow() {
    if(ui) delete ui;
    if(pOut) delete pOut;
    if(pConsole) delete pConsole;
    deletePlots();
}


void
MainWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event)
    if(ui) delete ui;
    ui = nullptr;
    if(pOut) delete pOut;
    pOut = nullptr;
    if(pConsole) delete pConsole;
    pConsole = nullptr;
    deletePlots() ;
}


void
MainWindow::deletePlots() {
    if(pPlotA) delete pPlotA;
    pPlotA = nullptr;
    if(pPlotV) delete pPlotV;
    pPlotV = nullptr;
}


void
MainWindow::on_Two_Gauss_clicked() {
    deletePlots();
    if(!readAlfaSFile())
        return;
    pPlotA = new Plot2D(nullptr, "AlfaS - Alfa");
    pPlotA->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlotA->UpdatePlot();
    pPlotA->show();

    pPlotV = new Plot2D(nullptr, "Alfa1 - Alfa2");
    pPlotV->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlotV->UpdatePlot();
    pPlotV->show();
}


void
MainWindow::on_Anarmonico_clicked() {
    deletePlots();
    pPlotA = new Plot2D(nullptr, "Cs - Ct");
    pPlotA->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlotA->UpdatePlot();
    pPlotA->show();
}


void
MainWindow::on_Debye_clicked() {
    deletePlots();
    pPlotA = new Plot2D(nullptr, "Cs - Ct");
    pPlotA->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlotA->UpdatePlot();
    pPlotA->show();
}


void
MainWindow::on_RamanSchottky_clicked() {
    deletePlots();
    pPlotA = new Plot2D(nullptr, "RamanSchottky");
    pPlotA->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlotA->UpdatePlot();
    pPlotA->show();
}


void
MainWindow::on_Schottky_clicked() {
    deletePlots();
    pPlotA = new Plot2D(nullptr, "Schottky");
    pPlotA->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlotA->UpdatePlot();
    pPlotA->show();
}


void
MainWindow::on_SummCos_clicked() {
    deletePlots();
    if(!readSummCosFile())
        return;
    SummCosFunction summCos(theTemperatures,
                            theAlfaS,
                            omega,
                            cost,
                            t0);

    MnUserParameters upar;
    upar.Add("BETA1",       1.108, 0.2088,   0.2,   5.0);
    upar.Add("BETA2",      10.520, 0.1315,   0.1,   1.0);
    upar.Add("TM(+3)",      2.172, 0.11327,  1.0,   7.0);
    upar.Add("TAU(-14)",    3.1,   0.90082,  0.1,  20.0);
    upar.Add("COST1(-3)",   9.446, 2.007,    0.1,  20.0);
    upar.Add("T00",       258.3,   4.2876, 100.0, 400.0);

    MnMigrad migrad(summCos, upar);
    FunctionMinimum min = migrad();
    std::cout << "minimum: " << min << std::endl;

    pPlotA = new Plot2D(nullptr, "SummCos");
    pPlotA->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlotA->UpdatePlot();
    pPlotA->show();
}


void
MainWindow::on_TwoGauss_T_clicked() {
    deletePlots();
    pPlotA = new Plot2D(nullptr, "AlfaS - Alfa");
    pPlotA->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlotA->UpdatePlot();
    pPlotA->show();
    pPlotV = new Plot2D(nullptr, "Alfa1 - Alfa2");
    pPlotV->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlotV->UpdatePlot();
    pPlotV->show();
}


void
MainWindow::on_exp_T_clicked() {
    deletePlots();
    pPlotA = new Plot2D(nullptr, "AlfaS - Alfa");
    pPlotA->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlotA->UpdatePlot();
    pPlotA->show();
}


bool
MainWindow::readAlfaSFile() {
    QString sDataDir =QString();
    statusBar()->showMessage("Select AlfaS File...", 0);
    QFileDialog chooseFileDialog;
    QString sFilename = chooseFileDialog.getOpenFileName(nullptr,
                                                         "Choose SummCos Data File",
                                                         sDataDir,
                                                         "Summ Cos Data files (*.dat *.txt)",
                                                         nullptr);
    statusBar()->clearMessage();
    if(sFilename == QString())
        return false;

    ifstream dataFile;
    dataFile.open(sFilename.toLatin1(), ios::in);
    if(!dataFile) {
        statusBar()->showMessage("Error in Opening Data File", 3000);
        return false;
    }
    try {
        dataFile >> nDati >> omega;
        theAlfaS.empty();
        theTemperatures.empty();
        double t, alfaS;
        while(!dataFile.eof()) {
            dataFile >> t >> alfaS;
            theTemperatures.push_back(t);
            theAlfaS.push_back(alfaS);
        }
    } catch(...) {
        statusBar()->showMessage("Error in Data File", 3000);
        dataFile.close();
        return false;
    }

    QString sSuccess = QString("Succesfully read %1 Data from %2")
                       .arg(theAlfaS.size())
                       .arg(sFilename);
    statusBar()->showMessage(sSuccess, 3000);
    dataFile.close();
    return true;
}


bool
MainWindow::readSummCosFile() {
    QString sDataDir =QString();
    statusBar()->showMessage("Choose SummCos Data File...", 0);
    QFileDialog chooseFileDialog;

    QString sFilename = chooseFileDialog.getOpenFileName(nullptr,
                                                         "Choose SummCos Data File",
                                                         sDataDir,
                                                         "Summ Cos Data files (*.dat *.txt)",
                                                         nullptr);
    statusBar()->clearMessage();
    if(sFilename == QString())
        return false;

    ifstream dataFile;
    dataFile.open(sFilename.toLatin1(), ios::in);
    if(!dataFile) {
      statusBar()->showMessage("Error in Opening Data File", 3000);
      return false;
    }
    try {
        theAlfaS.empty();
        theTemperatures.empty();
        double t, alfaS;
        dataFile >> omega;
        omega *= acos(-1.0);
        while(!dataFile.eof()) {
            dataFile >> t >> alfaS;
            theTemperatures.push_back(t);
            theAlfaS.push_back(alfaS);
        }
        dataFile.close();
        cost = theAlfaS[0];
        t0   = theTemperatures[0];
    } catch(...) {
        statusBar()->showMessage("Error in Data File", 3000);
        dataFile.close();
        return false;
    }

    QString sSuccess = QString("Succesfully read %1 Data from %2")
                       .arg(theAlfaS.size())
                       .arg(sFilename);
    statusBar()->showMessage(sSuccess, 3000);
    dataFile.close();
    return true;
}

