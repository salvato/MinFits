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
    paramNumber.setText(QString("").arg(num));
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
    paramNumber.setText(QString("%1").arg(num));
    editName.setText(sName);
    editInitialValue.setText(QString("%1").arg(initialValue));
    editErrorValue.setText(QString("%1").arg(errorValue));
    editMinValue.setText(QString("%1").arg(minValue));
    editMaxValue.setText(QString("%1").arg(maxValue));
    checkFixed.setChecked(bFixed);

    SetLayout();
    ConnectSignals();
    SetStyles();
}


ParameterLine::ParameterLine() {
    paramNumber.setText("N°");
    editName.setText("Name");
    editInitialValue.setText("Start");
    editErrorValue.setText("Error");
    editMinValue.setText("Min");
    editMaxValue.setText("Max");
    checkFixed.setChecked(false);
    checkFixed.setDisabled(true);

    paramNumber.setAlignment( Qt::AlignCenter);
    editName.setAlignment( Qt::AlignCenter);
    editInitialValue.setAlignment( Qt::AlignCenter);
    editErrorValue.setAlignment( Qt::AlignCenter);
    editMinValue.setAlignment( Qt::AlignCenter);
    editMaxValue.setAlignment( Qt::AlignCenter);

    SetLayout();
}

void
ParameterLine::SetLayout() {
    auto* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(&paramNumber);
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
    connect(&checkFixed, SIGNAL(FixedstateChanged(int)),
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
    emit parametersChanged(number);
}


void
ParameterLine::onInitialValueChanged(QString sNewInitialValue) {
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
ParameterLine::onErrorValueChanged(QString sNewErrorValue) {
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
ParameterLine::onMinValueChanged(QString sNewMinValue) {
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
ParameterLine::onMaxValueChanged(QString sNewMaxValue) {
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
ParameterLine::onFixedstateChanged(int newState) {
    emit parametersChanged(number);
}

