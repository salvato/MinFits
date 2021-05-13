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

#include <QMainWindow>
#include <QTextEdit>
#include <QSettings>

#include "parameterswindow.h"
#include "MinimizationFunction.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

protected:
    void saveSettings();
    void getSettings();
    bool getSummCosInitialValues();
    bool getSummSinInitialValues();
    bool readParamFile(QString sWindowTitle, QString sFileFilter);

private slots:
    void on_SummCos_clicked();
    void onFitDone();
    void on_SummSin_clicked();

private:
    Ui::MainWindow*       pUi;
    ParametersWindow*     pParams;
    MinimizationFunction* pFunctionToMinimize;

private:
    QString   sDataDir;
    QSettings settings;
};
