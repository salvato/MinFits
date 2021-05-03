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
