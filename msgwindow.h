#ifndef MSGWINDOW_H
#define MSGWINDOW_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>

class MsgWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MsgWindow(QWidget *parent = nullptr);

signals:

private:
    QTextEdit textEdit;
};

#endif // MSGWINDOW_H
