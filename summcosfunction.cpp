#include "summcosfunction.h"
#include "plot2d.h"
#include "math.h"
#include <QDebug>


//COMMON /BLOCCO1/ OMEGA, TAU, T1, BETA2, TM, BETA1, COST1, T00
static double Tau, T1, Beta2, Tm, Beta1, Cost1, T00;
static double Omega, costk, t0k;


double gammln(double xx);
double summTerm(int n);
int dceul(double (*termine)(int),
           double eps,
           int iter,
           int maxterm,
           double* vk);


using namespace ROOT;
using namespace Minuit2;


SummCosFunction::SummCosFunction(const std::vector<double>& temperatures,
                                 const std::vector<double>& measurements,
                                 std::vector<double>* fittedData,
                                 const double omega,
                                 const double cost,
                                 const double t0,
                                 Plot2D* pPlot)
    : theMeasurements(measurements)
    , theTemperatures(temperatures)
    , theFit(fittedData)
    , pPlotA(pPlot)
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
        theFit->at(j) =  costk - Cost1*(T1-t0k) - (Beta1*summa);
        diff = theFit->at(j) - theMeasurements[j];
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
        theFit->at(j) =  costk - Cost1*(T1-t0k) - (Beta1*summa);
    }

    if(pPlotA) {
        pPlotA->ClearDataSet(1);
        pPlotA->ClearDataSet(2);
        pPlotA->NewDataSet(1, 1, QColor(255,0,0), Plot2D::iplus, "Exper.");
        pPlotA->NewDataSet(2, 1, QColor(255,255,0), Plot2D::iline, "Theory");
        for(unsigned long i=0; i<theMeasurements.size(); i++) {
            pPlotA->NewPoint(1, theTemperatures[i], theMeasurements[i]);
            pPlotA->NewPoint(2, theTemperatures[i], theFit->at(i));
        }
        pPlotA->SetShowTitle(1, true);
        pPlotA->SetShowTitle(2, true);
        pPlotA->SetShowDataSet(1, true);
        pPlotA->SetShowDataSet(2, true);
        pPlotA->UpdatePlot();
    }
}


// Calcola una sommatoria infinita troncandola
// quando non ha senso calcolare ulteriori termini
// restituisce un valore diverso da 0 in caso di errore
int
dceul(double (*termine)(int),
      double eps,
      int iter,
      int maxterm,
      double* vk)
{
    *vk  = 0.0;
    double vk1;
    for(int j=0; j<iter; j++) {
        int k = j * maxterm;
        for(int i=k+1; i<=k+maxterm; i++) {
            vk1 = *vk + termine(i);
            if(*vk == vk1)
                return 0; // Sum converged !
            double perc = abs(vk1-*vk)/abs(vk1);
            if(perc < eps) { // Sum almost converged !
                *vk = vk1;
                return 0;
            }
            *vk = vk1;
        }
    }
    return -maxterm;// Sum does not converge !
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


// gammln() i.e. ln(gamma(xx)) as implemented in
// the second edition of "Numerical Recipes in C"
double
gammln(double xx) {
    double x, y, tmp, ser;
    static double coef[6]={76.18009172947146,     -86.50532032941677,
                           24.01409824083091,     -1.231739572450155,
                           0.1208650973866179e-2, -0.5395239384953e-5};

    y    = x = xx;
    tmp  = x + 5.5;
    tmp -= (x+0.5)*log(tmp);
    ser  = 1.000000000190015;
    for(int j=0; j<6; j++)
        ser += coef[j]/++y;
    return -tmp+log(2.5066282746310005*ser/x);
}
