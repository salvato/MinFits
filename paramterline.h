#ifndef PARAMTERLINE_H
#define PARAMTERLINE_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>

class ParamterLine : public QWidget
{
    Q_OBJECT
public:
    explicit ParamterLine(int num,
                          QString sName,
                          QString sInitialValue,
                          QString sErrorValue,
                          QString sMinValue,
                          QString sMaxValue,
                          bool bFixed,
                          QWidget *parent = nullptr);

signals:
    void parametersChanged(int);

public slots:
    void onNameChanged(QString sNewName);
    void onInitialValueChanged(QString sNewInitialValue);
    void onErrorValueChanged(QString sNewErrorValue);
    void onMinValueChanged(QString sNewMinValue);
    void onMaxValueChanged(QString sNewMaxValue);
    void onFixedstateChanged(int newState);

private:
    int number;
    QTextEdit paramNumber;
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

#endif // PARAMTERLINE_H
