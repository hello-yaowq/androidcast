#ifndef CONTROLTRHEAD_H
#define CONTROLTRHEAD_H

#include <QThread>
#include <QProcess>
#include <QQueue>

QT_BEGIN_NAMESPACE
class QProcess;
QT_END_NAMESPACE

class ControlThread :public QThread
{
    Q_OBJECT

public:
    ControlThread();
    ~ControlThread();
    void pushCmd(QString cmd);
    void stop();

signals:
    void capChanged();

private:
    void run();
    void screenCap();

    QProcess *proc;
    QQueue<QString> *queue;
    bool bStop;


};

#endif // CONTROLTRHEAD_H
