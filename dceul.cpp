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

