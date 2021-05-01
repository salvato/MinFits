#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plot2d.h"
#include "summcosfunction.h"

#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnUserParameters.h"
#include "Minuit2/MnPrint.h"

#include <QFileDialog>
#include <QTextStream>
#include <QFile>
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
    //pOut = new QDebugStream(std::cout, pConsole);
    getSettings();
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
    saveSettings();
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
    pPlotA = new Plot2D(nullptr, "SummCos");
    pPlotA->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlotA->UpdatePlot();
    pPlotA->show();

    theFit.clear();
    theFit.resize(theAlfaS.size());
    SummCosFunction summCos(theTemperatures,
                            theAlfaS,
                            &theFit,
                            omega,
                            cost,
                            t0,
                            pPlotA);

    MnUserParameters upar;
    upar.Add("BETA1",       1.108, 0.20880,   0.2,   5.0);
    upar.Add("BETA2",      10.520, 0.13150,   0.45,  0.58);
    upar.Add("TM(+3)",      2.172, 0.11327,   1.0,   7.0);
    upar.Add("TAU(-14)",    3.100, 0.90082,   0.1,  20.0);
    upar.Add("COST1(-3)",   9.446, 2.00700,   0.1,  20.0);
    upar.Add("T00",       258.300, 4.28760, 100.0, 400.0);

    MnMigrad migrad(summCos, upar);
    FunctionMinimum min = migrad();
    std::cout << "minimum: " << min << std::endl;
    summCos.Plot(min.UserParameters().Params());
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
    statusBar()->showMessage("Choose AlfaS Data File...", 0);
    QFileDialog chooseFileDialog(nullptr,
                                 "Open AlfaS Data File",
                                 sDataDir,
                                 "AlfaS Data files (*.dat *.txt)");
    chooseFileDialog.setFileMode(QFileDialog::ExistingFile);
    if(!chooseFileDialog.exec())
        return false;
    statusBar()->clearMessage();

    QString sFilename = chooseFileDialog.selectedFiles().at(0);
    if(sFilename == QString())
        return false;

    QFile dataFile(sFilename);
    if(!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        statusBar()->showMessage("Error Opening Data File", 3000);
        return false;
    }
    QTextStream inFileStream(&dataFile);
    try {
        inFileStream >> nDati >> omega;
        theAlfaS.empty();
        theTemperatures.empty();
        double t, alfaS;
        while(!inFileStream.atEnd()) {
            inFileStream >> t >> alfaS;
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
    statusBar()->showMessage("Choose SummCos Data File...", 0);
    QFileDialog chooseFileDialog(nullptr,
                                 "Open SUMM-COS Data File",
                                 sDataDir,
                                 "Summ Cos Data files (*.dat *.txt)");
    chooseFileDialog.setFileMode(QFileDialog::ExistingFile);
    if(!chooseFileDialog.exec())
        return false;
    statusBar()->clearMessage();

    QString sFilename = chooseFileDialog.selectedFiles().at(0);
    if(sFilename == QString())
        return false;

    sDataDir = chooseFileDialog.directory().absolutePath();
    settings.setValue("Data_Dir", sDataDir);
    QFile dataFile(sFilename);
    if(!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      statusBar()->showMessage("Error Opening Data File", 3000);
      return false;
    }
    QTextStream inFileStream(&dataFile);
    try {
        theAlfaS.clear();
        theTemperatures.clear();
        double t, alfaS;
        inFileStream >> omega;
        omega *= 2.0*M_PI;
        while(!inFileStream.atEnd()) {
            inFileStream >> t >> alfaS;
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

