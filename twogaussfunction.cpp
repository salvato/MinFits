#include "twogaussfunction.h"
#include <QtGlobal>

using namespace ROOT;
using namespace Minuit2;

TwoGaussFunction::TwoGaussFunction(const std::vector<double>& pos,
                                   const std::vector<double>& meas)
    : theMeasurements(meas)
    , thePositions(pos)
{}


TwoGaussFunction::~TwoGaussFunction(){

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

double
TwoGaussFunction::Up() const {
    return 1.0;
}


void
TwoGaussFunction::setErrorDef(double def) {
    theErrorDef = def;
}
