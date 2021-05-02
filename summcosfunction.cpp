#include "summcosfunction.h"
#include "dceul.h"
#include "gammln.h"
#include "plot2d.h"
#include "math.h"
#include <QDebug>
#include <QFileDialog>

//COMMON /BLOCCO1/ OMEGA, TAU, T1, BETA2, TM, BETA1, COST1, T00
static double Tau, T1, Beta2, Tm, Beta1, Cost1, T00;
static double Omega, costk, t0k;
std::vector<double> theFit;


double summTerm(int n);

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
SummCosFunction::getSettings() {
    sDataDir = settings.value("Data_Dir", QDir::currentPath()).toString();
}


void
SummCosFunction::saveSettings() {
    settings.setValue("Data_Dir", sDataDir);
}


bool
SummCosFunction::readDataFile() {
    //>>>statusBar()->showMessage("Choose SummCos Data File...", 0);
    QFileDialog chooseFileDialog(nullptr,
                                 "Open SUMM-COS Data File",
                                 sDataDir,
                                 "Summ Cos Data files (*.dat *.txt)");
    chooseFileDialog.setFileMode(QFileDialog::ExistingFile);
    if(!chooseFileDialog.exec())
        return false;
    //>>>statusBar()->clearMessage();

    QString sFilename = chooseFileDialog.selectedFiles().at(0);
    if(sFilename == QString())
        return false;

    sDataDir = chooseFileDialog.directory().absolutePath();
    settings.setValue("Data_Dir", sDataDir);
    QFile dataFile(sFilename);
    if(!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      //>>>statusBar()->showMessage("Error Opening Data File", 3000);
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
        costk = theMeasurements[0];
        t0k   = theTemperatures[0];
        saveSettings();
    } catch(...) {
        //>>>statusBar()->showMessage("Error in Data File", 3000);
        dataFile.close();
        return false;
    }

    QString sSuccess = QString("Succesfully read %1 Data from %2")
                       .arg(theMeasurements.size())
                       .arg(sFilename);
    //>>>statusBar()->showMessage(sSuccess, 3000);
    dataFile.close();
    return true;
}


double
SummCosFunction::operator()(const std::vector<double>& par) const
{
    double diff, summa;

    Beta1 = par[0];
    Beta2 = par[1];
    Tm    = par[2]*1.0e+03;
    Tau   = par[3]*1.0e-14;
    Cost1 = par[4]*1.0e-03;
    T00   = par[5];

    double Eps  = 1.0e-15;
    int Iter    = 300;
    int Maxterm = 100;
    double f    = 0.0;
    for(unsigned long j=0; j<theMeasurements.size(); j++) {
        T1 = theTemperatures[j];
        if(dceul(summTerm, Eps, Iter, Maxterm, &summa)) {
            qDebug() << "La Serie non converge";
            summa = __DBL_MAX__;
        }
        theFit[j] =  costk - Cost1*(T1-t0k) - (Beta1*summa);
        diff = theFit[j] - theMeasurements[j];
        f += (diff * diff);
    }
    return f;
}


void
SummCosFunction::Plot(const std::vector<double>& par) const
{

    Beta1 = par[0];
    Beta2 = par[1];
    Tm    = par[2]*1.0e+03;
    Tau   = par[3]*1.0e-14;
    Cost1 = par[4]*1.0e-03;
    T00   = par[5];

    double summa;
    double eps  = 1.0e-15;
    int maxIter = 300;
    int maxterm = 100;
    for(unsigned long j=0; j<theMeasurements.size(); j++) {
        T1 = theTemperatures[j];
        dceul(summTerm, eps, maxIter, maxterm, &summa);
        theFit[j] =  costk - Cost1*(T1-t0k) - (Beta1*summa);
    }

    if(pPlot) {
        pPlot->ClearDataSet(1);
        pPlot->ClearDataSet(2);
        pPlot->NewDataSet(1, 1, QColor(255,0,0), Plot2D::iplus, "Exper.");
        pPlot->NewDataSet(2, 1, QColor(255,255,0), Plot2D::iline, "Theory");
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


//----------------------------------------------------------------
//     QUESTO E'IL VECCHIO SUMM1 MODIFICATO CON LA RETTA ED IL COS
//     CIOE' LA PARTE REALE DEL MODULO PIU' LA VARIAZIONE
//     ANARMONICA DELLO STESSO E CON IL TEMPO DI RILASSAMENTO
//     ESPRESSO DAL VTF:TM = D*T0
//----------------------------------------------------------------
double
summTerm(int n) {
// COMMON /BLOCCO1/ OMEGA, TAU, T1, BETA2, TM, BETA1, COST1, T00

    double term  = Omega * Tau * exp(Tm/(T1-T00));
    double term1 = double(n)*Beta2;
    double term2 = pow(term, term1);
    term2 = 1.0/term2;
    double term3 = cos(term1 * M_PI_2);
    double term4 = gammln(term1+1.0)-gammln(double(n)+1.0);

//    qDebug() << "n="          << n
//             << "term="       << term
//             << "term1="      << term1
//             << "term2="      << term2
//             << "term3="      << term3
//             << "term4="      << term4
//             << "exp(term4)=" << exp(term4);

    double result = term2 * term3 * exp(term4);
    assert(qIsFinite(result));

    // (-1)^(n-1) : n > 0
    if((n-1)/2*2 != (n-1))
        result= -result;

    return result;

}

