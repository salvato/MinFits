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

#include "MinimizationFunction.h"
#include "qglobal.h"
#include <vector>
#include <QSettings>


QT_FORWARD_DECLARE_CLASS(Plot2D)


namespace ROOT {

   namespace Minuit2 {


class TwoGaussFunction : public MinimizationFunction
{
public:
    TwoGaussFunction();
    ~TwoGaussFunction();
    double operator()(const std::vector<double>& par) const;
    double Up() const;
    void SetErrorDef(double def);
    void Plot(const std::vector<double>& par) const;
    bool readDataFile();
    void saveSettings();
    bool saveData(QFile* pOutFile);

protected:
    void getSettings();

private:
    std::vector<double> theMeasurements;
    std::vector<double> theTemperatures;
    double theErrorDef;
    Plot2D*   pPlot;
    QString   sDataDir;
    QSettings settings;
};

   }  // namespace Minuit2

 }  // namespace ROOT
