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

private slots:
    void on_SummCos_clicked();
    void onFitDone();

public:
    QTextEdit* pConsole;

private:
    Ui::MainWindow*   pUi;
    QDebugStream*     pOut;
    ParametersWindow* pParams;

private:
    QString   sDataDir;
    QSettings settings;
};
