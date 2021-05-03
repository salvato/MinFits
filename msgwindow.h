#pragma once

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSettings>


class MsgWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MsgWindow(QWidget *parent = nullptr);
    ~MsgWindow();

protected:
    void saveSettings();
    void getSettings();

signals:

public:
    QTextEdit textEdit;

private:
    QVBoxLayout* pGeneralLayout;
    QSettings settings;
};
