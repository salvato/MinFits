/*
 *
Copyright (C) 2021  Gabriele Salvato

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/
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


void
MsgWindow::addLine(QString sString) {
    textEdit.insertPlainText(sString);
}
