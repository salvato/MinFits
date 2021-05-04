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

#include "dceul.h"
#include "gammln.h"
#include "plot2d.h"
#include "math.h"
#include <QDebug>
#include <QFileDialog>
#include <QApplication>
#include <iostream>

static double Tau, T1, Beta2, Tm, Beta1, T00;
static double Omega, t0k;

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
        inFileStream >> Omega;
        Omega *= 2.0*M_PI;
        while(!inFileStream.atEnd()) {
            inFileStream >> t >> alfaS;
            theTemperatures.push_back(t);
            theMeasurements.push_back(alfaS);
            theFit.push_back(0.0);
        }
        dataFile.close();
        t0k   = theTemperatures[0];
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
//    double Beta,  VM,  V0,  Tau, Flow1;
//    double Beta1, VM1, V01, Tau1, Flow2;
//    double STS;
    double f = 0.0;

//    Beta  = par[0]*1.0e-3;
//    VM    = par[1];
//    V0    = par[2];
//    Tau   = par[3]*1.0e-14;
//    Flow1 = par[4];
//    VM1   = par[5];
//    V01   = par[6];
//    Beta1 = par[7]*1.0e-3;
//    Tau1  = par[8]*1.0e-14;
//    Flow2 = par[9];
//    STS   = 0.0;

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
/*
    if(iflag==3 || !div(nCall,100).rem) {
      if(pPlotA != NULL) {
        pPlotA->DelDataSet(1);
        pPlotA->DelDataSet(2);
        pPlotA->NewDataSet(1, 1, RGB(255,0,0), CPlotWindow::iline, "Valori Teorici");
        pPlotA->NewDataSet(2, 1, RGB(255,255,0), CPlotWindow::iline, "Dati Sperimentali");
        for(int i=0; i<nDati; i++) {
          pPlotA->NewPoint(1, T[i], Alfa[i]);
          pPlotA->NewPoint(2, T[i], AlfaS[i]);
        }
        pPlotA->SetShowTitle(1, true);
        pPlotA->SetShowTitle(2, true);
        pPlotA->SetShowDataSet(1, true);
        pPlotA->SetShowDataSet(2, true);
        pPlotA->UpdatePlot();
      }
      if(pPlotV != NULL) {
        pPlotV->DelDataSet(1);
        pPlotV->DelDataSet(2);
        pPlotV->NewDataSet(1, 1, RGB(255,0,0), CPlotWindow::iline, "Alfa 1");
        pPlotV->NewDataSet(2, 1, RGB(255,255,0), CPlotWindow::iline, "Alfa 2");
        for(int i=0; i<nDati; i++) {
          pPlotV->NewPoint(1, T[i], Alfa1[i]);
          pPlotV->NewPoint(2, T[i], Alfa2[i]);
        }
        pPlotV->SetShowTitle(1, true);
        pPlotV->SetShowTitle(2, true);
        pPlotV->SetShowDataSet(1, true);
        pPlotV->SetShowDataSet(2, true);
        pPlotV->UpdatePlot();
      }
    }
*/
    return f;
}

