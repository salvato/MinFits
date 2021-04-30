#pragma once

#include "Minuit2/FCNBase.h"
#include <vector>

namespace ROOT {

   namespace Minuit2 {


class SummCosFunction : public FCNBase
{
public:
    SummCosFunction(const std::vector<double>& tempertures,
                    const std::vector<double>& measurements,
                    const double omega,
                    const double cost,
                    const double t0);
    ~SummCosFunction();
    double operator()(const std::vector<double>& par) const;
    double Up() const;
    void setErrorDef(double def);

protected:

private:
    std::vector<double> theMeasurements;
    std::vector<double> theTemperatures;
    double theErrorDef;

};

   }  // namespace Minuit2

 }  // namespace ROOT
