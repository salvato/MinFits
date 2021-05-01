#pragma once

#include "Minuit2/FCNBase.h"
#include "qglobal.h"
#include <vector>


QT_FORWARD_DECLARE_CLASS(Plot2D)


namespace ROOT {

   namespace Minuit2 {


class SummCosFunction : public FCNBase
{
public:
    SummCosFunction(const std::vector<double>& tempertures,
                    const std::vector<double>& measurements,
                    std::vector<double>* fittedData,
                    const double omega,
                    const double cost,
                    const double t0,
                    Plot2D *pPlot);
    ~SummCosFunction();
    double operator()(const std::vector<double>& par) const;
    double Up() const;
    void setErrorDef(double def);
    void Plot(const std::vector<double>& par) const;

protected:

private:
    std::vector<double> theMeasurements;
    std::vector<double> theTemperatures;
    std::vector<double>* theFit;
    double theErrorDef;
    Plot2D* pPlotA;
};

   }  // namespace Minuit2

 }  // namespace ROOT
