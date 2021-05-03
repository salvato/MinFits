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

#include "Minuit2/MnConfig.h"
#include <vector>
#include "Minuit2/GenericFunction.h"
#include "Minuit2/FCNBase.h"
#include <QFile>

namespace ROOT {

namespace Minuit2 {


// Interface (abstract class) defining the function to be minimized, which has to be implemented by the user.


class MinimizationFunction : public FCNBase {

public:
    virtual ~MinimizationFunction() {};
    virtual double operator()(const std::vector<double>& x) const = 0;
    virtual double ErrorDef() const {return Up();}
    virtual double Up() const = 0;
    virtual void SetErrorDef(double ) {};
    virtual void Plot(const std::vector<double>& par) const = 0;
    virtual bool readDataFile() = 0;
    virtual void saveSettings() = 0;
    virtual bool saveData(QFile* pOutFile) = 0;
};

}  // namespace Minuit2

}  // namespace ROOT

