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
#include "twogaussfunction.h"

#include "krab.h"
#include "gammln.h"
#include "plot2d.h"
#include "math.h"
#include <QDebug>
#include <QFileDialog>
#include <QApplication>
#include <iostream>

static double Beta,  Vm,  V0,  Tau, Flow1;
static double Beta1, Vm1, V01, Tau1, Flow2;
static double sts;

extern std::vector<double> theFit;


double summTerm(int n);

using namespace ROOT;
using namespace Minuit2;

TwoGaussFunction::TwoGaussFunction()
    : pPlot(nullptr)
{
    getSettings();
    pPlot = new Plot2D(nullptr, "Two Gauss");
    pPlot->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlot->UpdatePlot();
    pPlot->show();
}


TwoGaussFunction::~TwoGaussFunction(){
    if(pPlot) delete pPlot;
}


double
TwoGaussFunction::Up() const
{
    return 1.0;
}


void
TwoGaussFunction::SetErrorDef(double def) {
    theErrorDef = def;
}


void
TwoGaussFunction::getSettings() {
    sDataDir = settings.value("Data_Dir", QDir::currentPath()).toString();
}


void
TwoGaussFunction::saveSettings() {
    settings.setValue("Data_Dir", sDataDir);
}


bool
TwoGaussFunction::readDataFile() {
    QFileDialog chooseFileDialog(nullptr,
                                 "Open SUMM-SIN Data File",
                                 sDataDir,
                                 "Summ Sin Data files (*.dat *.txt)");
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
    QTextStream inFileStream(&dataFile);
    try {
        theMeasurements.clear();
        theTemperatures.clear();
        theFit.clear();
        double t, alfaS;
        while(!inFileStream.atEnd()) {
            inFileStream >> t >> alfaS;
            theTemperatures.push_back(t);
            theMeasurements.push_back(alfaS);
            theFit.push_back(0.0);
        }
        dataFile.close();
        saveSettings();
    } catch(...) {
        dataFile.close();
        return false;
    }

    dataFile.close();
    return true;
}


double
TwoGaussFunction::operator()(const std::vector<double>& par) const {
    Q_UNUSED(par);
    double f = 0.0;

    Beta  = par[0]*1.0e-3;
    Vm    = par[1];
    V0    = par[2];
    Tau   = par[3]*1.0e-14;
    Flow1 = par[4];
    Vm1   = par[5];
    V01   = par[6];
    Beta1 = par[7]*1.0e-3;
    Tau1  = par[8]*1.0e-14;
    Flow2 = par[9];
    sts   = 0.0;

//    double Diff;
//    for(int j=0; j<theMeasurements.size(); j++) {
//      T1 = T[j];
//      VMax      = (log(1.0e10)-log(Omeg*Omeg*Tau*Tau))*T1*0.5;
//      STS = 1.0;
//      KRAB(&Flow1, &VMax, TWOGAUSS, &VK, &VG);
//      Alfa1[j] = VK / T[j];
//      STS = 2.0;
//      KRAB(&Flow2, &VMax, TWOGAUSS, &VK, &VG);
//      Alfa2[j] = VK / T[j];
//      Alfa[j]  = Alfa1[j] + Alfa2[j];
//      Diff     = Alfa[j] - AlfaS[j];
//      Diff1[j] = Diff;
//      f      += Diff*Diff;
//    }

    return f;
}


void
TwoGaussFunction::Plot(const std::vector<double>& par) const
{
    Beta  = par[0]*1.0e-3;
    Vm    = par[1];
    V0    = par[2];
    Tau   = par[3]*1.0e-14;
    Flow1 = par[4];
    Vm1   = par[5];
    V01   = par[6];
    Beta1 = par[7]*1.0e-3;
    Tau1  = par[8]*1.0e-14;
    Flow2 = par[9];
    sts   = 0.0;

//    double summa;
//    double eps  = 1.0e-15;
//    int maxIter = 300;
//    int maxterm = 100;
//    for(ulong j=0; j<theMeasurements.size(); j++) {
//        T1 = theTemperatures[j];
//        dceul(summTerm, eps, maxIter, maxterm, &summa);
//        theFit[j] =  Beta1*summa;
//    }

    if(pPlot) {
        pPlot->ClearDataSet(1);
        pPlot->ClearDataSet(2);
        pPlot->NewDataSet(1, 1, QColor(255,  0,  0), Plot2D::iplus, "Exper.");
        pPlot->NewDataSet(2, 1, QColor(255,255,  0), Plot2D::iline, "Theory");
        for(unsigned long i=0; i<theMeasurements.size(); i++) {
            pPlot->NewPoint(1, theTemperatures[i], theMeasurements[i]);
            pPlot->NewPoint(2, theTemperatures[i], theFit[i]);
        }
        pPlot->SetShowTitle(1, true);
        pPlot->SetShowTitle(2, true);
        pPlot->SetShowDataSet(1, true);
        pPlot->SetShowDataSet(2, true);
        pPlot->UpdatePlot();
    }
}


bool
TwoGaussFunction::saveData(QFile* pOutFile) {
    pOutFile->write(QString("%1 %2 %3\n")
                    .arg("Temperature", 12)
                    .arg("Measurements", 12)
                    .arg("Fit", 12)
                    .toLocal8Bit());
    for(ulong i=0; i<theMeasurements.size(); i++) {
        pOutFile->write(QString("%1 %2 %3\n")
                        .arg(theTemperatures[i], 12, 'g', 6, ' ')
                        .arg(theMeasurements[i], 12, 'g', 6, ' ')
                        .arg(theFit[i], 12, 'g', 6, ' ')
                        .toLocal8Bit());
    }
    return true;
}


