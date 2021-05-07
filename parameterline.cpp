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
#include "parameterline.h"
#include <QHBoxLayout>

ParameterLine::ParameterLine(int num,
                             QString sName,
                             QString sInitialValue,
                             QString sErrorValue,
                             QString sMinValue,
                             QString sMaxValue,
                             bool bFixed,
                             QWidget *parent)
    : QWidget(parent)
    , number(num)
{
    editName.setText(sName);
    editInitialValue.setText(sInitialValue);
    editErrorValue.setText(sErrorValue);
    editMinValue.setText(sMinValue);
    editMaxValue.setText(sMaxValue);
    checkFixed.setChecked(bFixed);

    SetLayout();
    ConnectSignals();
    SetStyles();
}


ParameterLine::ParameterLine(int num,
                             QString sName,
                             double initialValue,
                             double errorValue,
                             double minValue,
                             double maxValue,
                             bool bFixed,
                             QWidget *parent)
    : QWidget(parent)
    , number(num)
{
    editName.setText(sName);
    editInitialValue.setText(QString("%1").arg(initialValue));
    editErrorValue.setText(QString("%1").arg(errorValue));
    editMinValue.setText(QString("%1").arg(minValue));
    editMaxValue.setText(QString("%1").arg(maxValue));
    checkFixed.setChecked(bFixed);
    checkFixed.setText("Fixed");

    SetLayout();
    ConnectSignals();
    SetStyles();
}


ParameterLine::ParameterLine() {
    editName.setText("Name");
    editInitialValue.setText("Start");
    editErrorValue.setText("Error");
    editMinValue.setText("Min");
    editMaxValue.setText("Max");
    checkFixed.setChecked(false);
    checkFixed.setDisabled(true);
    checkFixed.setText("Fixed");

    editName.setReadOnly(true);
    editInitialValue.setReadOnly(true);
    editErrorValue.setReadOnly(true);
    editMinValue.setReadOnly(true);
    editMaxValue.setReadOnly(true);

    editName.setAlignment( Qt::AlignCenter);
    editInitialValue.setAlignment( Qt::AlignCenter);
    editErrorValue.setAlignment( Qt::AlignCenter);
    editMinValue.setAlignment( Qt::AlignCenter);
    editMaxValue.setAlignment( Qt::AlignCenter);

    SetLayout();
}

void
ParameterLine::SetLayout() {
    checkFixed.setText("Fixed");
    auto* mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(&editName);
    mainLayout->addWidget(&editInitialValue);
    mainLayout->addWidget(&editErrorValue);
    mainLayout->addWidget(&editMinValue);
    mainLayout->addWidget(&editMaxValue);
    mainLayout->addWidget(&checkFixed);
    setLayout(mainLayout);
}


void
ParameterLine::ConnectSignals() {
    connect(&editName, SIGNAL(textChanged(QString)),
            this, SLOT(onNameChanged(QString)));
    connect(&editInitialValue, SIGNAL(textChanged(QString)),
            this, SLOT(onInitialValueChanged(QString)));
    connect(&editErrorValue, SIGNAL(textChanged(QString)),
            this, SLOT(onErrorValueChanged(QString)));
    connect(&editMinValue, SIGNAL(textChanged(QString)),
            this, SLOT(onMinValueChanged(QString)));
    connect(&editMaxValue, SIGNAL(textChanged(QString)),
            this, SLOT(onMaxValueChanged(QString)));
    connect(&checkFixed, SIGNAL(stateChanged(int)),
            this, SLOT(onFixedstateChanged(int)));
}


void
ParameterLine::SetStyles() {
    sNormalStyle = editInitialValue.styleSheet();

    sErrorStyle  = "QLineEdit { ";
    sErrorStyle +=     "color: rgb(255, 255, 255);";
    sErrorStyle +=     "background: rgb(255, 0, 0);";
    sErrorStyle +=     "selection-background-color: rgb(128, 128, 255);";
    sErrorStyle += "}";
}


void
ParameterLine::onNameChanged(QString sNewName) {
    Q_UNUSED(sNewName)
    emit nameChanged(number);
}


void
ParameterLine::onInitialValueChanged(QString sNewInitialValue) {
    bool ok;
    sNewInitialValue.toDouble(&ok);
    if(ok) {
        editInitialValue.setStyleSheet(sNormalStyle);
    }
    else {
        editInitialValue.setStyleSheet(sErrorStyle);
    }
    emit valueChanged(number);
}



void
ParameterLine::onErrorValueChanged(QString sNewErrorValue) {
    bool ok;
    sNewErrorValue.toDouble(&ok);
    if(ok) {
        editErrorValue.setStyleSheet(sNormalStyle);
    }
    else {
        editErrorValue.setStyleSheet(sErrorStyle);
    }
    emit errorChanged(number);
}



void
ParameterLine::onMinValueChanged(QString sNewMinValue) {
    bool ok;
    sNewMinValue.toDouble(&ok);
    if(ok) {
        editMinValue.setStyleSheet(sNormalStyle);
    }
    else {
        editMinValue.setStyleSheet(sErrorStyle);
    }
    emit minChanged(number);
}



void
ParameterLine::onMaxValueChanged(QString sNewMaxValue) {
    bool ok;
    sNewMaxValue.toDouble(&ok);
    if(ok) {
        editMaxValue.setStyleSheet(sNormalStyle);
    }
    else {
        editMaxValue.setStyleSheet(sErrorStyle);
    }
    emit maxChanged(number);
}


void
ParameterLine::onFixedstateChanged(int newState) {
    Q_UNUSED(newState)
    emit fixedstateChanged(number);
}


void
ParameterLine::setInitialValue(double newValue) {
    editInitialValue.setText(QString("%1").arg(newValue));
}


void
ParameterLine::setErrorValue(double newValue) {
    editErrorValue.setText(QString("%1").arg(newValue));
}


void
ParameterLine::setMinValue(double newValue) {
    editMinValue.setText(QString("%1").arg(newValue));
}


void
ParameterLine::setMaxValue(double newValue) {
    editMaxValue.setText(QString("%1").arg(newValue));
}


QString
ParameterLine::getName() {
    return editName.text();
}


double
ParameterLine::getInitialValue() {
    return editInitialValue.text().toDouble();
}


double
ParameterLine::getErrorValue() {
    return editErrorValue.text().toDouble();
}


double
ParameterLine::getMinValue() {
    return editMinValue.text().toDouble();
}


double
ParameterLine::getMaxValue() {
    return editMaxValue.text().toDouble();
}

bool
ParameterLine::isFixed() {
    return checkFixed.isChecked();
}
