#include "mainwindow.h"

#include <QApplication>
#include <QMutex>
#include <QThread>

static const QStringList s_logLetters = {"D", "W", "C", "F", "I"};
static QMutex s_logMutex;
static QFile s_logFile;

#ifdef _WIN32
#include <Windows.h>
#include <DbgHelp.h>
#include <strsafe.h>

static QString s_dumpsDirPath;

int generateDump(EXCEPTION_POINTERS *pExceptionPointers) {
    WCHAR szPath[MAX_PATH];
    WCHAR szFileName[MAX_PATH];
    DWORD dwBufferSize = MAX_PATH;
    HANDLE hDumpFile;
    SYSTEMTIME stLocalTime;
    MINIDUMP_EXCEPTION_INFORMATION ExpParam;

    GetLocalTime(&stLocalTime);
    GetTempPath(dwBufferSize, szPath);

    StringCchPrintf(szFileName,
                    MAX_PATH,
                    L"%s/%02d%02d%02d-%02d%02d%02d-%s.dmp",
                    s_dumpsDirPath.toStdWString().data(),
                    stLocalTime.wYear,
                    stLocalTime.wMonth,
                    stLocalTime.wDay,
                    stLocalTime.wHour,
                    stLocalTime.wMinute,
                    stLocalTime.wSecond,
                    QCoreApplication::applicationVersion().toStdWString().data());

    hDumpFile = CreateFile(szFileName,
                           GENERIC_READ|GENERIC_WRITE,
                           FILE_SHARE_WRITE|FILE_SHARE_READ,
                           nullptr,
                           CREATE_ALWAYS,
                           0,
                           nullptr);

    ExpParam.ThreadId = GetCurrentThreadId();
    ExpParam.ExceptionPointers = pExceptionPointers;
    ExpParam.ClientPointers = TRUE;
    MiniDumpWriteDump(GetCurrentProcess(),
                      GetCurrentProcessId(),
                      hDumpFile,
                      MiniDumpWithDataSegs,
                      &ExpParam,
                      nullptr,
                      nullptr);

    return EXCEPTION_EXECUTE_HANDLER;
}

LONG WINAPI crashHandler(EXCEPTION_POINTERS *ExceptionInfo) {
    generateDump(ExceptionInfo);
    return EXCEPTION_CONTINUE_SEARCH;
}
#endif



void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message) {
    QMutexLocker mutexLocker(&s_logMutex);
    Q_UNUSED(context)

    const QString dateTimeString = QDateTime::currentDateTime().toString("yy-MM-dd - hh:mm:ss.zzz");
    const QString threadIdString = QString::number(reinterpret_cast<qint64>(QThread::currentThreadId())).rightJustified(5, QLatin1Char('0'));
    const QString threadName = !QThread::currentThread()->objectName().isEmpty()
                               ? QThread::currentThread()->objectName()
                               : QLatin1String("Qt internal    ");

    const QString _message = QString("%1 | %2 - %3 | %4 | %5")
                             .arg(dateTimeString, threadIdString, threadName, s_logLetters.at(type), message);

    QTextStream logTextStream(&s_logFile);
    logTextStream << _message << Qt::endl;
    s_logFile.flush();
}



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

#ifdef _WIN32
    s_dumpsDirPath = QString("%1/dumps").arg(qApp->applicationDirPath());
    QDir().mkpath(s_dumpsDirPath);
    ::SetErrorMode(SEM_NOGPFAULTERRORBOX);
    ::SetUnhandledExceptionFilter(reinterpret_cast<LPTOP_LEVEL_EXCEPTION_FILTER>(crashHandler));
#endif

    const QString logsDirPath = QString("%1/logs").arg(qApp->applicationDirPath());
    QDir().mkpath(logsDirPath);
    const QString logFileName = QString("%1/%2.log").arg(logsDirPath, QDateTime::currentDateTime().toString("yyMMdd-hhmmss"));
    s_logFile.setFileName(logFileName);
    s_logFile.open(QFile::WriteOnly | QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHandler);

    MainWindow w;
    w.show();

    const int result = a.exec();

    qInstallMessageHandler(nullptr);
    s_logFile.close();

    return result;
}
