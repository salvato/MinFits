#include "msgwindow.h"

MsgWindow::MsgWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowFlags(windowFlags() |  Qt::WindowMinMaxButtonsHint);
    setMouseTracking(true);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pGeneralLayout = new QVBoxLayout();
    pGeneralLayout->addWidget(&textEdit);
    setLayout(pGeneralLayout);

    textEdit.setFontFamily("Courier");
    textEdit.document()->setMaximumBlockCount(10000);

    getSettings();
}


MsgWindow::~MsgWindow() {
    saveSettings();
}


void
MsgWindow::getSettings() {
    restoreGeometry(settings.value(QString("MsgWindow")).toByteArray());
}


void
MsgWindow::saveSettings() {
    settings.setValue(QString("MsgWindow"), saveGeometry());
}

