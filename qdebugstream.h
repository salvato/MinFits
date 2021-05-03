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
