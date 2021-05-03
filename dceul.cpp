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
