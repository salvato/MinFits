#pragma once

#include <QObject>
#include <QWidget>
#include <QPushButton>

#include "parameterline.h"
#include "summcosfunction.h"
#include "Minuit2/MnUserParameters.h"


QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)


using namespace ROOT;
using namespace Minuit2;


class ParametersWindow : public QWidget
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

protected:


signals:
    void closing();

public slots:
    void onClose();
    void onFit();
    void onNameChanged(int paramNum);
    void onValueChanged(int paramNum);
    void onErrorChanged(int paramNum);
    void onMinChanged(int paramNum);
    void onMaxChanged(int paramNum);
    void onFixedstateChanged(int paramNum);


private:
    bool bInitialized;
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
