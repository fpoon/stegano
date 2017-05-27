#ifndef LOGHELPER_H
#define LOGHELPER_H

#include <QPlainTextEdit>
#include <string>

void registerLogPte(QPlainTextEdit *pte);
#define unregisterLogPte registerLogPte(nullptr)

void log(std::string msg);
void log(const char* msg);

#endif // LOGHELPER_H
