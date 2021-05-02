#pragma once

#include <QObject>
#include <QWidget>
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

signals:
    void parametersChanged(int);
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
    QLineEdit paramNumber;
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
