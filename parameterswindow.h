#pragma once

#include <QObject>
#include <QDialog>
#include <QPushButton>

#include "parameterline.h"
#include "summcosfunction.h"
#include "Minuit2/MnUserParameters.h"


QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)


using namespace ROOT;
using namespace Minuit2;


class ParametersWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ParametersWindow(QString title="Fit Parameters", QWidget *parent = nullptr);
    ~ParametersWindow();
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
    void onClose();
    void onFit();

private:
    int nParams;
    MnUserParameters upar;
    QVBoxLayout* pParamLayout;
    QHBoxLayout* pButtonLayout;
    QVBoxLayout* pGeneralLayout;
    QPushButton buttonFit;
    QPushButton buttonClose;
    SummCosFunction* pSummCos;
    std::vector<ParameterLine*> parLine;
};
