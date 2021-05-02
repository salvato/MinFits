#pragma once

#include <QObject>
#include <QWidget>
#include "parameterline.h"
#include "Minuit2/MnUserParameters.h"


using namespace ROOT;
using namespace Minuit2;


class ParametersWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ParametersWindow(QString title="Fit Parameters", QWidget *parent = nullptr);

public:
    void Add(QString sName,
             double initVal,
             double error,
             double minVal,
             double maxVal,
             bool bFixed);
    MnUserParameters& getParams();

signals:

public slots:

private:
    int nParams;
    MnUserParameters upar;
};
