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
