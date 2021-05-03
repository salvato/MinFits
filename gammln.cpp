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
#include "gammln.h"
#include "math.h"


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
