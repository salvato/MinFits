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
#define _USE_MATH_DEFINES
#include "summcosfunction.h"

#include "dceul.h"
#include "gammln.h"
#include "plot2d.h"
#include "math.h"
#include <QDebug>
#include <QFileDialog>
#include <QApplication>
#include <iostream>
#include <cmath>


static double Tau_a, Beta2_a, Tm_a, Beta1_a, T00_a;
static double Tau_b, Beta2_b, Tm_b, Beta1_b, T00_b;
static double Cost1, T1;
static double Omega, costk, t0k;

extern std::vector<double> theFit;


double summCosTerm_a(int n);
double summCosTerm_b(int n);

using namespace ROOT;
using namespace Minuit2;


SummCosFunction::SummCosFunction()
    : pPlot(nullptr)
{
    getSettings();

    pPlot = new Plot2D(nullptr, "SummCos");
    pPlot->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlot->UpdatePlot();
    pPlot->show();
}


SummCosFunction::~SummCosFunction() {
    if(pPlot) delete pPlot;
}


double
SummCosFunction::Up() const
{
    return 1.0;
}


void
SummCosFunction::SetErrorDef(double def) {
    theErrorDef = def;
}


void
SummCosFunction::getSettings() {
    sDataDir = settings.value("Data_Dir", QDir::currentPath()).toString();
}


void
SummCosFunction::saveSettings() {
    settings.setValue("Data_Dir", sDataDir);
}


bool
SummCosFunction::readDataFile() {
    QFileDialog chooseFileDialog(nullptr,
                                 "Open SUMM-COS Data File",
                                 sDataDir,
                                 "Summ Cos Data files (*.dat *.txt)");
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
    QString sLine;
    QStringList sData;
    bool ok;
    try {
        theMeasurements.clear();
        theTemperatures.clear();
        theFit.clear();
        double t, alfaS;
        sLine = inFileStream.readLine();
        Omega = sLine.toDouble(&ok);
        if(!ok) {
            dataFile.close();
            return false;
        }
        Omega *= 2.0*M_PI;
        while(!inFileStream.atEnd()) {
            sLine = inFileStream.readLine();
            if(sLine.length() == 0)
                continue;
            QStringList sData(sLine.split(QRegExp("\\s+")));
            if(sData.size() < 2)
                continue;
            t = sData.at(0).toDouble(&ok);
            if(!ok)
                continue;
            alfaS = sData.at(1).toDouble(&ok);
            if(!ok)
                continue;
            theTemperatures.push_back(t);
            theMeasurements.push_back(alfaS);
            theFit.push_back(0.0);
        }
        dataFile.close();
        if(theMeasurements.size() == 0)
            return false;
        costk = theMeasurements[0];
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
SummCosFunction::operator()(const std::vector<double>& par) const
{
    double diff, summa;

    Beta1_a = par[0];
    Beta2_a = par[1];
    Tm_a    = par[2]*1.0e+03;
    Tau_a   = par[3]*1.0e-14;
    T00_a   = par[4];

    Beta1_b = par[5];
    Beta2_b = par[6];
    Tm_b    = par[7]*1.0e+03;
    Tau_b   = par[8]*1.0e-14;
    T00_b   = par[9];

    Cost1 = par[10]*1.0e-03;

    double eps  = 1.0e-15;
    int maxIter = 300;
    int maxTerm = 100;
    double f    = 0.0;
    for(ulong j=0; j<theMeasurements.size(); j++) {
        T1 = theTemperatures[j];
        if(dceul(summCosTerm_a, eps, maxIter, maxTerm, &summa)) {
            std::cout << "La Serie non converge";
            summa =  std::numeric_limits<double>::max();
        }
        theFit[j] =  costk - Cost1*(T1-t0k) - (Beta1_a*summa);
        if(dceul(summCosTerm_b, eps, maxIter, maxTerm, &summa)) {
            std::cout << "La Serie non converge";
            summa =  std::numeric_limits<double>::max();
        }
        theFit[j] -=  Beta1_b*summa;
        diff = theFit[j] - theMeasurements[j];

        f += (diff * diff);
    }
    return f;
}


void
SummCosFunction::Plot(const std::vector<double>& par) const
{
    double summa;

    Beta1_a = par[0];
    Beta2_a = par[1];
    Tm_a    = par[2]*1.0e+03;
    Tau_a   = par[3]*1.0e-14;
    T00_a   = par[4];

    Beta1_b = par[5];
    Beta2_b = par[6];
    Tm_b    = par[7]*1.0e+03;
    Tau_b   = par[8]*1.0e-14;
    T00_b   = par[9];

    Cost1 = par[10]*1.0e-03;

    double eps  = 1.0e-15;
    int maxIter = 300;
    int maxTerm = 100;
    for(ulong j=0; j<theMeasurements.size(); j++) {
        T1 = theTemperatures[j];
        if(dceul(summCosTerm_a, eps, maxIter, maxTerm, &summa)) {
            std::cout << "La Serie non converge";
            summa =  std::numeric_limits<double>::max();
        }
        theFit[j] =  costk - Cost1*(T1-t0k) - (Beta1_a*summa);
        if(dceul(summCosTerm_b, eps, maxIter, maxTerm, &summa)) {
            std::cout << "La Serie non converge";
            summa =  std::numeric_limits<double>::max();
        }
        theFit[j] -=  Beta1_b*summa;
    }

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
SummCosFunction::saveData(QFile* pOutFile) {
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


//----------------------------------------------------------------
//     QUESTO E'IL VECCHIO SUMM1 MODIFICATO CON LA RETTA ED IL COS
//     CIOE' LA PARTE REALE DEL MODULO PIU' LA VARIAZIONE
//     ANARMONICA DELLO STESSO E CON IL TEMPO DI RILASSAMENTO
//     ESPRESSO DAL VTF:TM = D*T0
//----------------------------------------------------------------

double
summCosTerm_a(int n) {
    double term  = Omega * Tau_a * exp(Tm_a/(T1-T00_a));
    double term1 = double(n)*Beta2_a;
    double term2 = pow(term, term1);
    term2 = 1.0/term2;
    if(!qIsFinite(term2)) return 0.0;

    double term3 = cos(term1 * M_PI_2);
    double term4 = gammln(term1+1.0)-gammln(double(n)+1.0);
    double result = term2 * term3 * exp(term4);
    if(!qIsFinite(result)) return 0.0;

    // (-1)^(n-1) : n > 0
    if((n-1)/2*2 != (n-1))
        result= -result;

    return result;
}


double
summCosTerm_b(int n) {
    double term  = Omega * Tau_b * exp(Tm_b/(T1-T00_b));
    double term1 = double(n)*Beta2_b;
    double term2 = pow(term, term1);
    term2 = 1.0/term2;
    if(!qIsFinite(term2)) return 0.0;

    double term3 = cos(term1 * M_PI_2);
    double term4 = gammln(term1+1.0)-gammln(double(n)+1.0);
    double result = term2 * term3 * exp(term4);
    if(!qIsFinite(result)) return 0.0;

    // (-1)^(n-1) : n > 0
    if((n-1)/2*2 != (n-1))
        result= -result;

    return result;
}
