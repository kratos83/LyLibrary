#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QProcess>

//Forward class declarations.
class QString;
class QStringList;

class Process : public QObject
{
    Q_OBJECT

public:
    explicit Process(QObject *parent = nullptr);
    ~Process();

    //Start programmi
    void start(const QString &program);
    void start(const QString &program, const QStringList &args);

    //Da bash
    int execute(const QString &program);
    int execute(const QString &program, const QStringList &args);

    //detach
    bool startDetached(const QString &program);
    bool startDetached(const QString &program , const QStringList &args);

    //process wait
    bool waitForStarted(int msecs = 30000);
    bool waitForReadyRead(int msecs = 30000);
    bool waitForBytesWritten(int msecs = 30000);
    bool waitForFinished(int msecs = 30000);

    QString readAllStandardOutput();
    QString readAllStandardError();
    QString errorString();
    QString readLine();

public slots:
    void readOutput();
    void readError();
    void readLineProcess();
    void exitCode(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess *m_process;
    QStringList parseCombinedArgString(const QString &program);
    QString m_readAllStandardOutput,m_readAllStandardError,m_errorString,m_line;

signals:
    void readyReadStandardOutput();
    void readyReadStandardError();
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif