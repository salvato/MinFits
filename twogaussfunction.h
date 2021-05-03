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
#pragma once

#include "Minuit2/FCNBase.h"
#include <vector>

namespace ROOT {

   namespace Minuit2 {


class TwoGaussFunction : public FCNBase
{
public:
    TwoGaussFunction(const std::vector<double>& pos,
                     const std::vector<double>& meas);
    ~TwoGaussFunction();
    double operator()(const std::vector<double>& par) const;
    double Up() const;
    void setErrorDef(double def);

private:
    std::vector<double> theMeasurements;
    std::vector<double> thePositions;
    double theErrorDef;
};

   }  // namespace Minuit2

 }  // namespace ROOT
