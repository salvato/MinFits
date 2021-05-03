#pragma once

#include "Minuit2/FCNBase.h"
#include "qglobal.h"
#include <vector>
#include <QSettings>


QT_FORWARD_DECLARE_CLASS(Plot2D)


namespace ROOT {

   namespace Minuit2 {


class SummSinFunction : public FCNBase
{
public:
    SummSinFunction();
    ~SummSinFunction();
    double operator()(const std::vector<double>& par) const;
    double Up() const;
    void setErrorDef(double def);
    void Plot(const std::vector<double>& par) const;
    bool readDataFile();

protected:
    void saveSettings();
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
