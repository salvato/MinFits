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
#pragma once

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QSettings>

#include <qdebugstream.h>
#include "msgwindow.h"

#include "parameterline.h"
//#include "summcosfunction.h"
#include "MinimizationFunction.h"
#include "Minuit2/MnUserParameters.h"


QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)


using namespace ROOT;
using namespace Minuit2;


class ParametersWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ParametersWindow(MinimizationFunction* pMyFunction,
                              QString title="Fit Parameters",
                              QWidget *parent = nullptr);
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
    void getSettings();
    void saveSettings();

signals:
    void closing();

public slots:
    void onClose();
    void onLoadData();
    void onSaveData();
    void onFit();
    void onNameChanged(int paramNum);
    void onValueChanged(int paramNum);
    void onErrorChanged(int paramNum);
    void onMinChanged(int paramNum);
    void onMaxChanged(int paramNum);
    void onFixedstateChanged(int paramNum);


private:
    int nParams;
    MnUserParameters upar;
    QVBoxLayout* pParamLayout;
    QHBoxLayout* pButtonLayout;
    QVBoxLayout* pGeneralLayout;
    QPushButton buttonFit;
    QPushButton buttonClose;
    QPushButton buttonLoadData;
    QPushButton buttonSaveData;
    MinimizationFunction* pFunction;
    std::vector<ParameterLine*> parLine;
    QString sDataDir;
    QSettings settings;
    QDebugStream* pOut;
    MsgWindow* pMsgWindow;
};
