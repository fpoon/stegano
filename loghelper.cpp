#include <string>
#include <iostream>
#include "loghelper.h"

using namespace std;

QPlainTextEdit *pteLog = nullptr;

void registerLogPte(QPlainTextEdit *pte)
{
    pteLog = pte;
}

void log(string msg)
{
    log(msg.c_str());
}

void log(const char *msg)
{
    cout << msg << endl;
    if (!pteLog)
        return;
    pteLog->moveCursor(QTextCursor::End);
    pteLog->insertPlainText(msg);
    pteLog->insertPlainText("\n");
}
