#pragma once

#include <QTextEdit>
#include <streambuf>

using namespace std;


class QDebugStream : public std::basic_streambuf<char>
{

public:
    QDebugStream(std::ostream& stream, QTextEdit* text_edit);
    virtual ~QDebugStream();
    static void registerMyConsoleMessageHandler();

private:
    static void myConsoleMessageHandler(QtMsgType type,
                                        const QMessageLogContext&,
                                        const QString& msg);

protected:
    virtual int
        overflow(int v) {
            if(v == '\n')
                log_window->append("");
            return v;
        }
    virtual std::streamsize xsputn(const char *p, std::streamsize n);

private:
    std::ostream&   m_stream;
    std::streambuf* m_old_buf;
    QTextEdit*      log_window;
};
