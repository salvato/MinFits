#include "paramterline.h"
#include <QHBoxLayout>

ParamterLine::ParamterLine(int num,
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
    auto* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(&paramNumber);
    mainLayout->addWidget(&editName);
    mainLayout->addWidget(&editInitialValue);
    mainLayout->addWidget(&editErrorValue);
    mainLayout->addWidget(&editMinValue);
    mainLayout->addWidget(&editMaxValue);
    mainLayout->addWidget(&checkFixed);

    paramNumber.setText(QString("").arg(num));
    editName.setText(sName);
    editInitialValue.setText(sInitialValue);
    editErrorValue.setText(sErrorValue);
    editMinValue.setText(sMinValue);
    editMaxValue.setText(sMaxValue);
    checkFixed.setChecked(bFixed);

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
    connect(&checkFixed, SIGNAL(FixedstateChanged(int)),
            this, SLOT(onFixedstateChanged(int)));


    sNormalStyle = editInitialValue.styleSheet();

    sErrorStyle  = "QLineEdit { ";
    sErrorStyle +=     "color: rgb(255, 255, 255);";
    sErrorStyle +=     "background: rgb(255, 0, 0);";
    sErrorStyle +=     "selection-background-color: rgb(128, 128, 255);";
    sErrorStyle += "}";

}

void
ParamterLine::onNameChanged(QString sNewName) {
    emit parametersChanged(number);
}


void
ParamterLine::onInitialValueChanged(QString sNewInitialValue) {
    bool ok;
    double dValue = sNewInitialValue.toDouble(&ok);
    if(ok) {
        editInitialValue.setStyleSheet(sNormalStyle);
    }
    else {
        editInitialValue.setStyleSheet(sErrorStyle);
    }
    emit parametersChanged(number);
}



void
ParamterLine::onErrorValueChanged(QString sNewErrorValue) {
    bool ok;
    double dValue = sNewErrorValue.toDouble(&ok);
    if(ok) {
        editErrorValue.setStyleSheet(sNormalStyle);
    }
    else {
        editErrorValue.setStyleSheet(sErrorStyle);
    }
    emit parametersChanged(number);
}



void
ParamterLine::onMinValueChanged(QString sNewMinValue) {
    bool ok;
    double dValue = sNewMinValue.toDouble(&ok);
    if(ok) {
        editMinValue.setStyleSheet(sNormalStyle);
    }
    else {
        editMinValue.setStyleSheet(sErrorStyle);
    }
    emit parametersChanged(number);
}



void
ParamterLine::onMaxValueChanged(QString sNewMaxValue) {
    bool ok;
    double dValue = sNewMaxValue.toDouble(&ok);
    if(ok) {
        editMaxValue.setStyleSheet(sNormalStyle);
    }
    else {
        editMaxValue.setStyleSheet(sErrorStyle);
    }
    emit parametersChanged(number);
}


void
ParamterLine::onFixedstateChanged(int newState) {
    emit parametersChanged(number);
}

