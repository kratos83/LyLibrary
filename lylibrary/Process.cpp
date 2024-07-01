#include "Process.h"

Process::Process(QObject *parent) :
    QObject(parent)
{
    m_process = new QProcess();
    QObject::connect(m_process, SIGNAL( readyReadStandardOutput() ), this,
                   SIGNAL( readyReadStandardOutput() ));
    QObject::connect(m_process,&QProcess::readyReadStandardOutput,this,&Process::readOutput);

    QObject::connect(m_process, SIGNAL( readyReadStandardError() ), this,
                   SIGNAL( readyReadStandardError() ));
    QObject::connect(m_process,&QProcess::readyReadStandardError,this,&Process::readError);

    QObject::connect(m_process, SIGNAL( finished ( int, QProcess::ExitStatus )), this,
                   SIGNAL( finished ( int, QProcess::ExitStatus )) );
    QObject::connect(m_process,static_cast<void (QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished),this,&Process::finished);

    QObject::connect(m_process,&QProcess::readyReadStandardOutput,this,&Process::readLineProcess);
}

Process::~Process()
{
    delete m_process;
}

void Process::start(const QString &program)
{
    //Questo serve per lo start del programma
    QStringList args = parseCombinedArgString(program);
    QString prog = args.first();
    args.removeFirst();
    m_process->start(prog,args);
}

void Process::start(const QString &program, const QStringList &args)
{
    m_process->start(program,args);
}

bool Process::waitForStarted(int msecs)
{
    return m_process->waitForStarted(msecs);
}

bool Process::waitForReadyRead(int msecs)
{
    return m_process->waitForReadyRead(msecs);
}

bool Process::waitForBytesWritten(int msecs)
{
    return m_process->waitForBytesWritten(msecs);
}

bool Process::waitForFinished(int msecs)
{
    return m_process->waitForFinished(msecs);
}

//Da bash
int Process::execute(const QString &program)
{
    QStringList args = parseCombinedArgString(program);
    QString prog = args.first();
    args.removeFirst();
    return QProcess::execute(prog,args);
}

int Process::execute(const QString &program, const QStringList &args)
{
   return QProcess::execute(program,args);
}

//detach
bool Process::startDetached(const QString &program)
{
    QStringList args = parseCombinedArgString(program);
    QString prog = args.first();
    args.removeFirst();
    return QProcess::startDetached(prog,args);
}

bool Process::startDetached(const QString &program , const QStringList &args)
{
    return QProcess::startDetached(program,args);
}

void Process::readOutput()
{
    if (m_process->isOpen())
        m_readAllStandardOutput = QString::fromUtf8(m_process->readAllStandardOutput());
}

void Process::readError()
{
    if (m_process->isOpen())
    {
        m_readAllStandardError = QString::fromUtf8(m_process->readAllStandardError());
        m_errorString = m_process->errorString();
    }
}

void Process::readLineProcess()
{
    if (m_process->isOpen())
        m_line = QString::fromUtf8(m_process->readLine());
}

void Process::exitCode(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (m_process->isOpen())
        m_readAllStandardOutput = QString::fromUtf8(m_process->readAllStandardOutput());
}

/*
 * Retrieves Standard output of member process
 */
QString Process::readAllStandardOutput()
{
  return m_readAllStandardOutput;
}

/*
 * Leggo linea per linea del processo
 */
QString Process::readLine()
{
    return m_line;
}
/*
 * Retrieves StandardError output of member process
 */
QString Process::readAllStandardError()
{
  return m_readAllStandardError;
}

/*
 * Retrieves ErrorString of member process
 */
QString Process::errorString()
{
  return m_errorString;
}

QStringList Process::parseCombinedArgString(const QString &program)
{
    QStringList args;
    QString tmp;
    int quoteCount = 0;
    bool inQuote = false;

    // handle quoting. tokens can be surrounded by double quotes
    // "hello world". three consecutive double quotes represent
    // the quote character itself.
    for (int i = 0; i < program.size(); ++i) {
        if (program.at(i) == QLatin1Char('"')) {
            ++quoteCount;
            if (quoteCount == 3) {
                // third consecutive quote
                quoteCount = 0;
                tmp += program.at(i);
            }
            continue;
        }
        if (quoteCount) {
            if (quoteCount == 1)
                inQuote = !inQuote;
            quoteCount = 0;
        }
        if (!inQuote && program.at(i).isSpace()) {
            if (!tmp.isEmpty()) {
                args += tmp;
                tmp.clear();
            }
        } else {
            tmp += program.at(i);
        }
    }
    if (!tmp.isEmpty())
        args += tmp;

    return args;
}