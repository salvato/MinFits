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
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>

class ParameterLine : public QWidget
{
    Q_OBJECT
public:
    explicit ParameterLine(int num,
                           QString sName,
                           QString sInitialValue,
                           QString sErrorValue,
                           QString sMinValue,
                           QString sMaxValue,
                           bool bFixed,
                           QWidget *parent = nullptr);

    explicit ParameterLine(int num,
                           QString sName,
                           double initialValue,
                           double errorValue,
                           double minValue,
                           double maxValue,
                           bool bFixed,
                           QWidget *parent = nullptr);

    explicit ParameterLine();

public:
    void setInitialValue(double newValue);
    void setErrorValue(double newValue);
    void setMinValue(double newValue);
    void setMaxValue(double newValue);

    QString getName();
    double getInitialValue();
    double getErrorValue();
    double getMinValue();
    double getMaxValue();
    bool   isFixed();

signals:
    void nameChanged(int);
    void valueChanged(int);
    void errorChanged(int);
    void minChanged(int);
    void maxChanged(int);
    void fixedstateChanged(int);

public slots:
    void onNameChanged(QString sNewName);
    void onInitialValueChanged(QString sNewInitialValue);
    void onErrorValueChanged(QString sNewErrorValue);
    void onMinValueChanged(QString sNewMinValue);
    void onMaxValueChanged(QString sNewMaxValue);
    void onFixedstateChanged(int newState);

protected:
    void SetLayout();
    void ConnectSignals();
    void SetStyles();

private:
    int number;
    QLineEdit editName;
    QLineEdit editInitialValue;
    QLineEdit editErrorValue;
    QLineEdit editMinValue;
    QLineEdit editMaxValue;
    QCheckBox checkFixed;
    // QLineEdit styles
    QString sNormalStyle;
    QString sErrorStyle;
};
