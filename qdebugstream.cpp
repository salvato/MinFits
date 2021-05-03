#include "qdebugstream.h"
#include <iostream>


using namespace std;



QDebugStream::QDebugStream(std::ostream &stream, QTextEdit *text_edit)
    : std::basic_streambuf<char>()
    , m_stream(stream)
{
    log_window = text_edit;
    m_old_buf = stream.rdbuf();
    stream.rdbuf(this);
}


QDebugStream::~QDebugStream() {
    m_stream.rdbuf(m_old_buf);
}


void
QDebugStream::registerMyConsoleMessageHandler() {
    qInstallMessageHandler(myConsoleMessageHandler);
}


void
QDebugStream::myConsoleMessageHandler(QtMsgType type,
                                      const QMessageLogContext&,
                                      const QString& msg)
{
    //QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
       case QtDebugMsg:
          // fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
           break;
       case QtInfoMsg:
          // fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
           break;
       case QtWarningMsg:
          // fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
           break;
       case QtCriticalMsg:
           //fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
           break;
       case QtFatalMsg:
          // fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
           break;
       default:
            std::cout << msg.toStdString().c_str();
           break;
       }
}


std::streamsize
QDebugStream::xsputn(const char *p, std::streamsize n) {
    QByteArray localMsg(p, n);
    log_window->insertPlainText(QString(localMsg));
//    int m = strlen(p) - ulong(n);
//    if(m) {
//        for(int i=0; i<m; i++)
//            log_window->insertPlainText(QString(*(p+n+i)));
//    }
    return n;
}
