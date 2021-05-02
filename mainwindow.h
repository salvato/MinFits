#pragma once

#include <QMainWindow>
#include <QTextEdit>
#include <QSettings>

#include "qdebugstream.h"
#include "parameterswindow.h"


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

private:

public:
    QTextEdit* pConsole;

private slots:
    void on_Two_Gauss_clicked();
    void on_Anarmonico_clicked();
    void on_Debye_clicked();
    void on_RamanSchottky_clicked();
    void on_Schottky_clicked();
    void on_SummCos_clicked();
    void on_TwoGauss_T_clicked();
    void on_exp_T_clicked();

private:
    Ui::MainWindow* ui;
    QDebugStream*   pOut;
    QString         sDataDir;
    QSettings       settings;
    ParametersWindow* pParams;
};
