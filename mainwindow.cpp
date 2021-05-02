#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <math.h>


using namespace ROOT;
using namespace Minuit2;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , pConsole(nullptr)
    , pUi(new Ui::MainWindow)
    , pOut(nullptr)
    , pParams(nullptr)
{
    pUi->setupUi(this);
    pConsole = new QTextEdit();
    pConsole->document()->setMaximumBlockCount(10000);
    pConsole->show();
    //pOut = new QDebugStream(std::cout, pConsole);
    getSettings();
}


MainWindow::~MainWindow() {
    if(pUi) delete pUi;
    if(pOut) delete pOut;
    if(pConsole) delete pConsole;
    if(pParams) delete pParams;
}


void
MainWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event)
    saveSettings();
    if(pUi) delete pUi;
    pUi = nullptr;
    if(pOut) delete pOut;
    pOut = nullptr;
    if(pConsole) delete pConsole;
    pConsole = nullptr;
    if(pParams) delete pParams;
    pParams = nullptr;
}


void
MainWindow::getSettings() {
    restoreGeometry(settings.value(QString("MainWindow_Dialog")).toByteArray());
    sDataDir = settings.value("Data_Dir",   QDir::currentPath()).toString();
}


void
MainWindow::saveSettings() {
    settings.setValue(QString("MainWindow_Dialog"), saveGeometry());
    settings.setValue("Data_Dir", sDataDir);
}


void
MainWindow::on_SummCos_clicked() {
    pParams = new ParametersWindow("Fit Parameters", nullptr);

    pParams->Add("BETA1",       1.108, 0.20880,   0.2,   5.00, false);
    pParams->Add("BETA2",      10.520, 0.13150,   0.45,  0.58, false);
    pParams->Add("TM(+3)",      2.172, 0.11327,   1.0,   7.00, false);
    pParams->Add("TAU(-14)",    3.100, 0.90082,   0.1,  20.00, false);
    pParams->Add("COST1(-3)",   9.446, 2.00700,   0.1,  20.00, false);
    pParams->Add("T00",       258.300, 4.28760, 100.0, 400.00, false);

    setDisabled(true);
    pParams->exec();
    delete pParams;
    setEnabled(true);
}

