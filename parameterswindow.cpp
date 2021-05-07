/*
 *
Copyright (C) 2021  Gabriele Salvato

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "parameterswindow.h"

#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnUserParameters.h"
#include "Minuit2/MnPrint.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>


ParametersWindow::ParametersWindow(MinimizationFunction* pMyFunction,
                                   QString title,
                                   QWidget *parent)
    : QWidget(parent)
    , pFunction(pMyFunction)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowFlags(windowFlags() |  Qt::WindowMinMaxButtonsHint);
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWindowTitle(title);

    buttonClose.setText("Close");
    buttonLoadData.setText("Load Data");
    buttonFit.setText("Start Fit");
    buttonSaveData.setText("Save Fit");
    buttonFit.setEnabled(false);
    buttonSaveData.setEnabled(false);

    connect(&buttonClose, SIGNAL(clicked()),
            this, SLOT(onClose()));
    connect(&buttonLoadData, SIGNAL(clicked()),
            this, SLOT(onLoadData()));
    connect(&buttonFit, SIGNAL(clicked()),
            this, SLOT(onFit()));
    connect(&buttonSaveData, SIGNAL(clicked()),
            this, SLOT(onSaveData()));

    pParamLayout = new QVBoxLayout();
    pParamLayout->addWidget(new ParameterLine());

    pButtonLayout = new QHBoxLayout();
    pButtonLayout->addWidget(&buttonLoadData);
    pButtonLayout->addWidget(&buttonFit);
    pButtonLayout->addWidget(&buttonSaveData);
    pButtonLayout->addWidget(&buttonClose);

    pGeneralLayout = new QVBoxLayout();
    pGeneralLayout ->addLayout(pParamLayout);
    pGeneralLayout ->addLayout(pButtonLayout);
    setLayout(pGeneralLayout);

    nParams = 0;
    getSettings();
}


ParametersWindow::~ParametersWindow() {
    saveSettings();
}


void
ParametersWindow::getSettings() {
    sDataDir = settings.value("Data_Dir", QDir::currentPath()).toString();
}


void
ParametersWindow::saveSettings() {
    settings.setValue("Data_Dir", sDataDir);
}


void
ParametersWindow::Add(QString sName, double initVal, double error, double minVal, double maxVal, bool bFixed) {
    ParameterLine* newPar = new ParameterLine(nParams, sName, initVal, error, minVal, maxVal, bFixed);
    parLine.push_back(newPar);
    pParamLayout->addWidget(parLine.back());
    upar.Add(sName.toStdString(), initVal, error, minVal, maxVal);
    nParams++;
    if(bFixed) upar.Fix(sName.toStdString());

    connect(newPar, SIGNAL(nameChanged(int)),
            this, SLOT(onNameChanged(int)));
    connect(newPar, SIGNAL(valueChanged(int)),
            this, SLOT(onValueChanged(int)));
    connect(newPar, SIGNAL(errorChanged(int)),
            this, SLOT(onErrorChanged(int)));
    connect(newPar, SIGNAL(minChanged(int)),
            this, SLOT(onMinChanged(int)));
    connect(newPar, SIGNAL(maxChanged(int)),
            this, SLOT(onMaxChanged(int)));
    connect(newPar, SIGNAL(fixedstateChanged(int)),
            this, SLOT(onFixedstateChanged(int)));
}


MnUserParameters&
ParametersWindow::getParams() {
    return upar;
}


void
ParametersWindow::onLoadData() {
    if(!pFunction->readDataFile())
        return;
    pFunction->Plot(upar.Params());
    buttonFit.setEnabled(true);
    buttonSaveData.setEnabled(true);
}


void
ParametersWindow::onSaveData() {
    QString outFileName = QFileDialog::getSaveFileName(this,
                                                       "Save Fit Data",
                                                       sDataDir,
                                                       "Fit Data (*.fit);;All Files (*)");
    if(outFileName == QString())
        return;

    QFile outFile(outFileName);
    if(!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this,
                                 "Unable to open file",
                                 outFile.errorString());
        return;
    }

    outFile.write(QString("Fit Parameters:\n").toLocal8Bit());
    for(ulong i=0; i<upar.Parameters().size(); i++) {
        outFile.write(QString("%1, %2, %3, %4, %5, ")
                      .arg(QString::fromStdString(upar.Parameters().at(i).GetName()), 12)
                      .arg(upar.Parameters().at(i).Value(), 12)
                      .arg(upar.Parameters().at(i).Error(), 12)
                      .arg(upar.Parameters().at(i).LowerLimit(), 12)
                      .arg(upar.Parameters().at(i).UpperLimit(), 12)
                      .toLocal8Bit());
        outFile.write(QString("%1\n")
                      .arg(upar.Parameters().at(i).IsFixed() ? "fixed" : "free", 12)
                      .toLocal8Bit());
    }
    pFunction->saveData(&outFile);
}


void
ParametersWindow::onFit() {
    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
    setDisabled(true);
    MnMigrad migrad(*pFunction, getParams());
    FunctionMinimum min = migrad();
    std::cout << "minimum: " << min << std::endl;
    pFunction->Plot(min.UserParameters().Params());
    MnUserParameterState optimum = min.UserState();
    if(qIsFinite(optimum.Edm())) {
        for(unsigned int i=0; i<optimum.Params().size(); i++) {
            parLine.at(i)->setInitialValue(optimum.Value(i));
            parLine.at(i)->setErrorValue(optimum.Error(i));
        }
    }
    QApplication::restoreOverrideCursor();
    setEnabled(true);
}


void
ParametersWindow::onClose() {
    emit closing();
}

void
ParametersWindow::onNameChanged(int paramNum) {
    upar.SetName(paramNum, parLine.at(paramNum)->getName().toStdString());
}


void
ParametersWindow::onValueChanged(int paramNum) {
    upar.SetValue(paramNum, parLine.at(paramNum)->getInitialValue());
    pFunction->Plot(upar.Params());
}


void
ParametersWindow::onErrorChanged(int paramNum) {
    upar.SetError(paramNum, parLine.at(paramNum)->getErrorValue());
}


void
ParametersWindow::onMinChanged(int paramNum) {
    upar.SetLowerLimit(paramNum, parLine.at(paramNum)->getMinValue());
}


void
ParametersWindow::onMaxChanged(int paramNum) {
    upar.SetUpperLimit(paramNum, parLine.at(paramNum)->getMaxValue());
}


void
ParametersWindow::onFixedstateChanged(int paramNum) {
    if(parLine.at(paramNum)->isFixed())
        upar.Fix(paramNum);
    else
        upar.Release(paramNum);
}
