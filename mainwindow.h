#pragma once

#include <QMainWindow>
#include <QTextEdit>
#include <QSettings>

#include "qdebugstream.h"
#include "parameterswindow.h"
#include "msgwindow.h"
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

private slots:
    void on_SummCos_clicked();
    void onFitDone();
    void on_SummSin_clicked();

private:
    Ui::MainWindow*       pUi;
    QDebugStream*         pOut;
    ParametersWindow*     pParams;
    MsgWindow*            pMsgWindow;
    MinimizationFunction* pFunctionToMinimize;

private:
    QString   sDataDir;
    QSettings settings;
};
