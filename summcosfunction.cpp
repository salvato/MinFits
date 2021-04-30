#include "summcosfunction.h"
#include "math.h"
#include <QDebug>

//COMMON /BLOCCO1/ OMEGA, TAU, T1, BETA2, TM, BETA1, COST1, T00
double Tau, T1, Beta2, Tm, Beta1, Cost1, T00;
std::vector<double> alfat;
double Omega, costk, t0k;

double gammln(double xx);
double summ3(int n);
void dceul(double eps, int iter, int maxterm, double* vk, int* ier);


using namespace ROOT;
using namespace Minuit2;


SummCosFunction::SummCosFunction(const std::vector<double>& temperatures,
                                 const std::vector<double>& measurements,
                                 const double omega,
                                 const double cost,
                                 const double t0)
    : theMeasurements(temperatures)
    , theTemperatures(measurements)
{
    Omega = omega;
    costk = cost;
    t0k = t0;
}


SummCosFunction::~SummCosFunction() {
}


double
SummCosFunction::Up() const
{
    return 1.0;
}


double
SummCosFunction::operator()(const std::vector<double>& par) const
{
    double diff, VK;

    Beta1 = par[0];
    Beta2 = par[1];
    Tm    = par[2]*1.0e+03;
    Tau   = par[3]*1.0e-14;
    Cost1 = par[4]*1.0e-03;
    T00   = par[5];

    int Ierr;
    double Eps  = 1.0e-15;
    int Iter    = 300;
    int Maxterm = 100;
    double f    = 0.0;
    for(unsigned long j=0; j<theMeasurements.size(); j++) {
        T1 = theTemperatures[j];
        dceul(Eps, Iter, Maxterm, &VK, &Ierr);
        if(Ierr != 0)
            qDebug() << "La Serie non converge";
        alfat[j] =  costk - Cost1*(T1-t0k) - (Beta1*VK);
        diff = alfat[j] - theMeasurements[j];
        f += (diff * diff);
    }
/*
    if(iflag==3 || !div(nCall,100).rem) {
        if(pPlotA != NULL) {
            pPlotA->DelDataSet(1);
            pPlotA->DelDataSet(2);
            pPlotA->NewDataSet(1, 1, RGB(255,0,0), CPlotWindow::iplus, "Dati Sperimentali");
            pPlotA->NewDataSet(2, 1, RGB(255,255,0), CPlotWindow::iline, "Valori Teorici");
            for(int k=0; k<nFiles; k++) {
                for(int i=0; i<nOss[k]; i++) {
                    pPlotA->NewPoint(1, T[i+NM*k], alfas[i+NM*k]);
                    pPlotA->NewPoint(2, T[i+NM*k], alfat[i+NM*k]);
                }
            }
            pPlotA->SetShowTitle(1, true);
            pPlotA->SetShowTitle(2, true);
            pPlotA->SetShowDataSet(1, true);
            pPlotA->SetShowDataSet(2, true);
            pPlotA->UpdatePlot();
        }
    }
*/
    return f;
}


void
dceul(double eps, int iter, int maxterm, double* vk, int* ier) {
    *ier = 0;
    *vk = 0.0;
    double vk1;
    int nterm = maxterm;
    for(int j=0; j<iter; j++) {
        int k = j * nterm;
        for(int i=k+1; i<=k+nterm; i++) {
            vk1 = *vk + summ3(i);
            if(*vk == vk1) return;
            if(abs(vk1-*vk)/abs(vk1) < eps) {
                *vk = vk1;
                return;
            }
            *vk = vk1;
        }
    }
    *ier = -maxterm;
    return;
}


//----------------------------------------------------------------
//     QUESTO E'IL VECCHIO SUMM1 MODIFICATO CON LA RETTA ED IL COS
//     CIOE' LA PARTE REALE DEL MODULO PIU' LA VARIAZIONE
//     ANARMONICA DELLO STESSO E CON IL TEMPO DI RILASSAMENTO
//     ESPRESSO DAL VTF:TM = D*T0
//----------------------------------------------------------------
double
summ3(int n) {

//COMMON /BLOCCO1/ OMEGA, TAU, T1, BETA2, TM, BETA1, COST1, T00

    double T;
    double an = double(n);

    // Sign(A, B) Returns `ABS(A)*s', where s is +1 if `B.GE.0', -1 otherwise.
    if(abs(T1-T00) < 1.0e-300)
        T = (T1-T00)>0.0 ? 300.0 : -300.0;//sign(-300.0, (T1-T00));
    else
        T = Tm/(T1-T00);

    if(T > 300.0) T = 300.0;
    if(T <-300.0) T =-300.0;

    double term = Omega * Tau * exp(T);
    double term1 = an * Beta2;
    if(term1 < 1.0e-300) term1 = 1.0e-300;
    if(term1 > 10.e300)  term1 = 1.0e300;

    double term2 = 1.0 / pow(term, term1);
    if (term2 > 1.0e80) term2 = 1.0e80;
    double term3 = cos(Beta2 * an * M_PI_2);
    double term4 = gammln(an*Beta2+1.0)-gammln(an+1.0);

    double result = term2 * term3 * exp(term4);

    if((n-1)/2*2 != (n-1))
        result= -result;

    return result;

}


// gammln() as implemented in the second
// edition of "Numerical Recipes in C"
double
gammln(double xx) {
    double x, y, tmp, ser;
    static double cof[6]={76.18009172947146,     -86.50532032941677,
                          24.01409824083091,     -1.231739572450155,
                          0.1208650973866179e-2, -0.5395239384953e-5};

    y=x=xx;
    tmp=x+5.5;
    tmp -= (x+0.5)*log(tmp);
    ser=1.000000000190015;
    for(int j=0; j<6; j++)
        ser += cof[j]/++y;
    return -tmp+log(2.5066282746310005*ser/x);
}
