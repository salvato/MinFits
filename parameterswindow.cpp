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
    , pOut(nullptr)
    , pMsgWindow(nullptr)
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

    pMsgWindow = new MsgWindow();
    pMsgWindow->show();
    pOut = new QDebugStream(std::cout, &pMsgWindow->textEdit);
}


ParametersWindow::~ParametersWindow() {
    saveSettings();
    if(pOut) delete pOut;
    if(pMsgWindow) delete pMsgWindow;
}


void
ParametersWindow::getSettings() {
    restoreGeometry(settings.value(QString("Parameters")).toByteArray());
    sDataDir = settings.value("Data_Dir", QDir::currentPath()).toString();
}


void
ParametersWindow::saveSettings() {
    settings.setValue(QString("Parameters"), saveGeometry());
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
    QString sPrint;
    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
    setDisabled(true);
    MnMigrad migrad(*pFunction, getParams());
    FunctionMinimum min = migrad();
    if(min.IsValid()) {
        sPrint = QString("Minuit did succesfully converge.\n");
    }
    else {
        sPrint = QString("Warning: Minuit did not converge.\n");
    }
    pMsgWindow->addLine(sPrint);
    pMsgWindow->addLine(QString("#of function calls: %1\n").arg(min.NFcn()));
    pMsgWindow->addLine(QString("minimum function Value: %1\n").arg(min.Fval()));
    pMsgWindow->addLine(QString("minimum edm: %1\n").arg(min.Edm()));
    pMsgWindow->addLine(QString("minimum internal state vector: LAVector parameters:\n"));
    for(unsigned int i=0; i<min.Parameters().Vec().size(); i++) {
        pMsgWindow->addLine(QString("%1\n"). arg(min.Parameters().Vec()(i)));
    }


    std::cout << "minimum: " << min << std::endl;
/*
minimum:
WARNING: Minuit did not converge.

# of function calls: 215
minimum function Value: 0.005456041350947
minimum edm: 0.169282631536
minimum internal state vector: LAVector parameters:
    -0.7783580832175
     -0.822843598405
    -0.1342995180886
     0.2308895355795
     -1.328947382139
     -1.136192288438
     0.2164091659714
     0.2826694940555
     0.9507547537307
     -1.245865715352

minimum internal covariance matrix: LASymMatrix parameters:
   0.0016231194 -0.00040719303   0.0023250739  0.00010040308  0.00058031577 -0.00046811724  4.4896943e-06     0.44549986  -0.0004812567   -0.014457851
 -0.00040719303   0.0011258121   -0.001262713 -5.4541741e-05 -0.00031502305  0.00025537256 -2.4394684e-06     -0.2420692  0.00026149928   0.0078558856
   0.0023250739   -0.001262713   0.0080734868  0.00031276393   0.0018057103  -0.0014604143  1.3997254e-05      1.3889146  -0.0015003922   -0.045074598
  0.00010040308 -5.4541741e-05  0.00031276393  0.00092890345  7.8016927e-05 -6.3099729e-05  6.0460145e-07     0.05999334 -6.4808578e-05  -0.0019469699
  0.00058031577 -0.00031502305   0.0018057103  7.8016927e-05   0.0013594836 -0.00036427421  3.4921859e-06     0.34652069 -0.00037433312   -0.011245676
 -0.00046811724  0.00025537256  -0.0014604143 -6.3099729e-05 -0.00036427421   0.0011553691 -2.8114673e-06    -0.27891896  0.00030128978   0.0090520653
  4.4896943e-06 -2.4394684e-06  1.3997254e-05  6.0460145e-07  3.4921859e-06 -2.8114673e-06  2.7277629e-08   0.0026804995 -2.8955165e-06 -8.6990066e-05
     0.44549986     -0.2420692      1.3889146     0.05999334     0.34652069    -0.27891896   0.0026804995      265.97744    -0.28734016     -8.6321947
  -0.0004812567  0.00026149928  -0.0015003922 -6.4808578e-05 -0.00037433312  0.00030128978 -2.8955165e-06    -0.28734016   0.0012259109   0.0093252884
   -0.014457851   0.0078558856   -0.045074598  -0.0019469699   -0.011245676   0.0090520653 -8.6990066e-05     -8.6321947   0.0093252884     0.28104548


# ext. ||   Name    ||   type  ||     Value     ||  Error +/-

   0||     Beta1_a|| limited ||      1.531996725902 ||0.1426828819553
   1||     Beta2_a|| limited ||     0.2201127246056 ||0.01026743316164
   2||    Tm_a(+3)|| limited ||      4.897467236865 ||0.4001567729907
   3||  Tau_a(-14)|| limited ||      24.61542870531 ||0.5918075420656
   4||       T00_a|| limited ||      15.67512091983 ||1.721572791622
   5||     Beta1_b|| limited ||     0.5124917530488 ||0.07118767778647
   6||     Beta2_b|| limited ||     0.6466257720584 ||7.258816125183e-05
   7||    Tm_b(+3)|| limited ||      6.755140866766 ||4.576320393865
   8||  Tau_b(-14)|| limited ||      36.28639329592 ||0.4057993161898
   9||       T00_b|| limited ||      20.20378830067 ||31.47802966528



WARNING: FunctionMinimum is invalid:
     Edm is above max

*/
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
