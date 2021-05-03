#pragma once

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>

class MsgWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MsgWindow(QWidget *parent = nullptr);

signals:

public:
    QTextEdit textEdit;

private:
    QVBoxLayout* pGeneralLayout;
};
