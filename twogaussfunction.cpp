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


double twoGauss(double v);


static double Beta,  Vm,  V0,  Tau,  Flow1;
static double Beta1, Vm1, V01, Tau1, Flow2;
static double Omega, T1, Sts;

extern std::vector<double> theFit;
std::vector<double> alfa1;
std::vector<double> alfa2;

double summTerm(int n);

using namespace ROOT;
using namespace Minuit2;

TwoGaussFunction::TwoGaussFunction()
    : pPlot1(nullptr)
    , pPlot2(nullptr)
{
    getSettings();
    pPlot1 = new Plot2D(nullptr, "Two Gauss Fit");
    pPlot1->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlot1->UpdatePlot();
    pPlot1->show();
    pPlot2 = new Plot2D(nullptr, "Two Gauss");
    pPlot2->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlot2->UpdatePlot();
    pPlot2->show();
}


TwoGaussFunction::~TwoGaussFunction(){
    if(pPlot1) delete pPlot1;
    if(pPlot2) delete pPlot2;
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
        alfa1.clear();
        alfa2.clear();
        double t, alfaS;
        int nDati;
        inFileStream >> nDati >> Omega;
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
    alfa1.resize(theMeasurements.size());
    alfa2.resize(theMeasurements.size());
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
    Sts   = 0.0;

    double diff, vk, vg;

    for(size_t j=0; j<theMeasurements.size(); j++) {
      T1 = theTemperatures[j];
      double vMax = (log(1.0e10)-log(Omega*Omega*Tau*Tau))*T1*0.5;
      Sts = 1.0;
      krab(Flow1, vMax, twoGauss, &vk, &vg);
      alfa1[j] = vk / theTemperatures[j];
      Sts = 2.0;
      krab(Flow2, vMax, twoGauss, &vk, &vg);
      alfa2[j] = vk / theTemperatures[j];
      theFit[j]  = alfa1[j] + alfa2[j];
      diff = theFit[j] - theMeasurements[j];
      f += diff*diff;
    }

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
    Sts   = 0.0;

    double vk, vg;

    for(size_t j=0; j<theMeasurements.size(); j++) {
      double t1 = theTemperatures[j];
      double vMax = (log(1.0e10)-log(Omega*Omega*Tau*Tau))*t1*0.5;
      Sts = 1.0;
      krab(Flow1, vMax, twoGauss, &vk, &vg);
      alfa1[j] = vk / theTemperatures[j];
      Sts = 2.0;
      krab(Flow2, vMax, twoGauss, &vk, &vg);
      alfa2[j] = vk / theTemperatures[j];
      theFit[j]  = alfa1[j] + alfa2[j];
    }

    if(pPlot1) {
        pPlot1->ClearDataSet(1);
        pPlot1->ClearDataSet(2);
        pPlot1->NewDataSet(1, 1, QColor(255,  0,  0), Plot2D::iplus, "Exper.");
        pPlot1->NewDataSet(2, 1, QColor(255,255,  0), Plot2D::iline, "Theory");
        for(size_t i=0; i<theMeasurements.size(); i++) {
            pPlot1->NewPoint(1, theTemperatures[i], theMeasurements[i]);
            pPlot1->NewPoint(2, theTemperatures[i], theFit[i]);
        }
        pPlot1->SetShowTitle(1, true);
        pPlot1->SetShowTitle(2, true);
        pPlot1->SetShowDataSet(1, true);
        pPlot1->SetShowDataSet(2, true);
        pPlot1->UpdatePlot();
    }
    if(pPlot2) {
      pPlot2->ClearDataSet(1);
      pPlot2->ClearDataSet(2);
      pPlot2->NewDataSet(1, 1, QColor(255,  0,  0), Plot2D::iline, "Alfa 1");
      pPlot2->NewDataSet(2, 1, QColor(255,255,  0), Plot2D::iline, "Alfa 2");
      for(size_t i=0; i<theMeasurements.size(); i++) {
        pPlot2->NewPoint(1, theTemperatures[i], alfa1[i]);
        pPlot2->NewPoint(2, theTemperatures[i], alfa2[i]);
      }
      pPlot2->SetShowTitle(1, true);
      pPlot2->SetShowTitle(2, true);
      pPlot2->SetShowDataSet(1, true);
      pPlot2->SetShowDataSet(2, true);
      pPlot2->UpdatePlot();
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


double
twoGauss(double v) {
    double Term11, T11, Grand1;
    double Term01= pow(v, T1);
    double ar1= Omega * Tau1 * Term01;

    if(ar1 < __FLT_MIN__) {
        Term11 = (v-Vm1)/(1.4142*V01);
        T11 = Term11*Term11;
        if(T11 > 150.0)
            Grand1 = 0.0;
        else {
            Grand1 = Beta1 * (ar1*exp(-T11)/(1.0+ar1*ar1));
            if(std::abs(Grand1) < 1.0e-15)
                Grand1 = 0.0;
        }
    }
    else {
        Grand1 = 0.0;
    }

    double Term = pow(v, T1);
    double ar = Omega * Tau * Term;
    IF( AR.LT.10.D+37) GOTO 41
  4077  TWOGAUSS= 0.D0
    GOTO 2020
  41 TERM1=(V-VM)/(1.4142 * V0 )
    T=TERM1*TERM1
    IF(T.GT.150.D0) GOTO 4066
    TWOGAUSS=BETA*(AR*DEXP(-T)/(1.D0+AR*AR))
    GOTO 2020
  4066 TWOGAUSS=0.D0
  C-------------------------------------------------------
  2020  CONTINUE
    IF (STS.EQ.1.) GRAND1 =0.D0
    IF (STS.EQ.2.) TWOGAUSS  =0.D0
}

/*
  TERM= DEXP(V/T1)
  AR= OMEG * TAU * TERM
  IF( AR.LT.10.D+37) GOTO 41
4077  TWOGAUSS= 0.D0
  GOTO 2020
41 TERM1=(V-VM)/(1.4142 * V0 )
  T=TERM1*TERM1
  IF(T.GT.150.D0) GOTO 4066
  TWOGAUSS=BETA*(AR*DEXP(-T)/(1.D0+AR*AR))
  GOTO 2020
4066 TWOGAUSS=0.D0
C-------------------------------------------------------
2020  CONTINUE
      IF (STS.EQ.1.) GRAND1 =0.D0
  IF (STS.EQ.2.) TWOGAUSS  =0.D0
C**
  TWOGAUSS=TWOGAUSS+GRAND1
  IF(DABS(TWOGAUSS).LT.1.D-25) TWOGAUSS=0.D0
  RETURN
  END
*/
