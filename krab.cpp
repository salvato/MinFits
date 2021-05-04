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
#include "krab.h"

// Kronrod Quadrature (n=20) of an integral from a to b.
// a = Lower limit of integral
// b = Upper limit of integral
// grand() = External Function defining Integrand.
// vk = Value of Integral using 41 nodes
// vg = Value of Integral using 20 nodes
void
krab(double a, double b, double(*grand)(double), double* vk, double* vg) {
    const double wgb[] =
    {
        0.88070036e-2, 0.20300715e-1,
        0.31336024e-1, 0.41638371e-1,
        0.50965060e-1, 0.59097266e-1,
        0.65844319e-1, 0.71048055e-1,
        0.74586493e-1, 0.76376694e-1
    };

    const double wkb[] =
    {
        0.15367919e-2, 0.43001349e-2,
        0.73130846e-2, 0.10194187e-1,
        0.12941067e-1, 0.15643653e-1,
        0.18300085e-1, 0.20834437e-1,
        0.23217411e-1, 0.25472287e-1,
        0.27597553e-1, 0.29555700e-1,
        0.31326619e-1, 0.32917299e-1,
        0.34324336e-1, 0.35527212e-1,
        0.36515345e-1, 0.37291438e-1,
        0.37852249e-1, 0.38188934e-1
    };

    const double abb[] =
    {
        0.92460613e-2, 0.18014036e-1,
        0.29588683e-1, 0.43882786e-1,
        0.60861594e-1, 0.80441514e-1,
        0.10247929,    0.12683405,
        0.15338117,    0.18197315,
        0.21242977,    0.24456650,
        0.27820341,    0.31314696,
        0.34918607,    0.38610707,
        0.42369726,    0.46173674
    };
    double s = b - a;
    *vg = 0.0;
    *vk = 0.08300356e-1 * grand(a+0.5*s);

    for(int j=0; j<20; j++) {
        double add = s * abb[j];
        add = grand(a+add) + grand(b-add);
        *vk += (wkb[j] * add);
        if(j & 1) // se j è dispari...
            *vg += (wgb[j/2] * add);
    }
    *vk *= s;
    *vg *= s;
}
