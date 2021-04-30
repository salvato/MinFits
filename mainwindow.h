#pragma once

#include <QMainWindow>
#include <QTextEdit>

#include "qdebugstream.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


QT_FORWARD_DECLARE_CLASS(Plot2D)


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private:
    void deletePlots();
    bool readAlfaSFile();
    bool readSummCosFile();

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
    Plot2D*         pPlotA;
    Plot2D*         pPlotV;

    std::vector<double> theAlfaS;
    std::vector<double> theTemperatures;
    int nDati;
    double omega, cost, t0;
};
