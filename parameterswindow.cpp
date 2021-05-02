#include "parameterswindow.h"

#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnUserParameters.h"
#include "Minuit2/MnPrint.h"


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>


ParametersWindow::ParametersWindow(QString title, QWidget *parent)
    : QDialog(parent)
    , pSummCos(nullptr)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowFlags(windowFlags() |  Qt::WindowMinMaxButtonsHint);
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setWindowTitle(title);
    buttonClose.setText("Close");
    buttonFit.setText("Start Fit");
    connect(&buttonClose, SIGNAL(clicked()),
            this, SLOT(onClose()));
    connect(&buttonFit, SIGNAL(clicked()),
            this, SLOT(onFit()));

    pParamLayout = new QVBoxLayout();
    pParamLayout->addWidget(new ParameterLine());

    pButtonLayout = new QHBoxLayout();
    pButtonLayout->addWidget(&buttonClose);
    pButtonLayout->addWidget(&buttonFit);

    pGeneralLayout = new QVBoxLayout();
    pGeneralLayout ->addLayout(pParamLayout);
    pGeneralLayout ->addLayout(pButtonLayout);
    setLayout(pGeneralLayout);

    nParams = 0;
    pSummCos = new SummCosFunction();
}


ParametersWindow::~ParametersWindow() {
    if(pSummCos) delete pSummCos;
}


void
ParametersWindow::Add(QString sName, double initVal, double error, double minVal, double maxVal, bool bFixed) {
    parLine.push_back(new ParameterLine(nParams, sName, initVal, error, minVal, maxVal, bFixed));
    pParamLayout->addWidget(parLine.back());
    upar.Add(sName.toStdString(), initVal, error, minVal, maxVal);
    if(bFixed)
        upar.Fix(sName.toStdString());
}


MnUserParameters&
ParametersWindow::getParams() {
    return upar;
}


void
ParametersWindow::onFit() {
    if(!pSummCos->readDataFile()) {
        return;
    }
    MnMigrad migrad(*pSummCos, getParams());
    FunctionMinimum min = migrad();
    std::cout << "minimum: " << min << std::endl;
    pSummCos->Plot(min.UserParameters().Params());
    MnUserParameterState optimum = min.UserState();
    for(unsigned int i=0; i<min.UserParameters().Params().size(); i++) {
        parLine.at(i)->setInitialValue(min.UserParameters().Value(i));
        parLine.at(i)->setErrorValue(min.UserParameters().Error(i));
    }
    optimum.Fval();

}


void
ParametersWindow::onClose() {
    accept();
}

