#ifndef LOG_H
#define LOG_H

#pragma once

#include <QDebug>

#define FUNCTION_LOG FunctionLog __function_log__(__FUNCTION__);

#define DEBUG_LOG qDebug().noquote() << __FUNCTION__ <<
#define DEBUG_LOG_ qDebug().noquote() << __FUNCTION__;

#define INFO_LOG qInfo().noquote() << __FUNCTION__ <<
#define INFO_LOG_ qInfo().noquote() << __FUNCTION__;

#define FUNCTION_LINE QString("%1:%2:").arg(__FUNCTION__).arg(__LINE__)

#define WARNING_LOG qWarning().noquote() << FUNCTION_LINE <<
#define WARNING_LOG_ qWarning().noquote() << FUNCTION_LINE;

#define CRITICAL_LOG qCritical().noquote() << FUNCTION_LINE <<
#define CRITICAL_LOG_ qCritical().noquote() << FUNCTION_LINE;

#define FATAL_LOG qFatal().noquote() << FUNCTION_LINE <<

#define dump(x) "{" << #x << ":" << x << "} "

class FunctionLog
{
public:
    FunctionLog(const char *function) : m_function(function) { qDebug("+ %s", function); }
    ~FunctionLog() { qDebug("- %s", m_function); }

private:
    const char* m_function;
};

#endif // LOG_H
