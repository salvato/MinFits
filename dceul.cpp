#include "dceul.h"
#include "math.h"

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


// From "Numerical Recipes"
//void
//eulsum(double *sum, double term, int jterm, double wksp[]) {
//    static int nterm;
//    double tmp, dum;

//    if (jterm == 1) {
//        nterm=1;
//        *sum=0.5*(wksp[1]=term);
//    } else {
//        tmp = wksp[1];
//        wksp[1] = term;
//        for(int j=1; j<=nterm-1; j++) {
//            dum = wksp[j+1];
//            wksp[j+1] = 0.5*(wksp[j]+tmp);
//            tmp = dum;
//        }
//        wksp[nterm+1] = 0.5*(wksp[nterm]+tmp);
//        if (abs(wksp[nterm+1]) <= abs(wksp[nterm]))
//            *sum += (0.5*wksp[++nterm]);
//        else
//            *sum += wksp[nterm+1];
//    }
//}
