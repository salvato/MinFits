#include "parameterswindow.h"


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>


ParametersWindow::ParametersWindow(QString title, QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowFlags(windowFlags() |  Qt::WindowMinMaxButtonsHint);
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setWindowTitle(title);
    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->addWidget(new ParameterLine());
    setLayout(pLayout);
    nParams = 0;
}


void
ParametersWindow::Add(QString sName, double initVal, double error, double minVal, double maxVal, bool bFixed) {
    QLayout* pLayout = this->layout();
    nParams++;
    pLayout->addWidget(new ParameterLine(nParams, sName, initVal, error, minVal, maxVal, bFixed));
    upar.Add(sName.toStdString(), initVal, error, minVal, maxVal);
    if(bFixed)
        upar.Fix(sName.toStdString());
    setLayout(pLayout);
}

MnUserParameters&
ParametersWindow::getParams() {
    return upar;
}
