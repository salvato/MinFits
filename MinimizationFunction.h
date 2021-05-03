#pragma once

#include "Minuit2/MnConfig.h"
#include <vector>
#include "Minuit2/GenericFunction.h"
#include "Minuit2/FCNBase.h"

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

};

}  // namespace Minuit2

}  // namespace ROOT

