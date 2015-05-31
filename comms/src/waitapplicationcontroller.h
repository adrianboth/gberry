#ifndef WAITAPPLICATIONCONTROLLER_H
#define WAITAPPLICATIONCONTROLLER_H

#include <QObject>
#include <QProcess>

class WaitApplicationController : public QObject
{
    Q_OBJECT
public:
    explicit WaitApplicationController(QString& pathToApp, QObject *parent = 0);
    ~WaitApplicationController();

    void launch();
    void pause();
    void resume();
    void stop();

    void setMessage(QString& msg);

signals:
    void launched();
    void resumed();

public slots:
    void onWaitApplicationRunning(QProcess::ProcessState processState);
    void onWaitApplicationFinished(int exitCode);

private:
    enum State { NOT_RUNNING, RUN, STOP};

    QString _pathToApp;
    QProcess _process;
    State _state;
};

#endif // WAITAPPLICATIONCONTROLLER_H
