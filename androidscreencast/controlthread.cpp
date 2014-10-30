#include "controlthread.h"



ControlThread::ControlThread()
{
    proc = new QProcess();
    queue = new QQueue<QString>;
    bStop =false;
}

ControlThread::~ControlThread()
{
    bStop = true;
    if (proc && proc->state() == QProcess::Running) {
        proc->terminate();
        proc->waitForFinished(3000);
    }
    delete proc;

    queue->clear();
    delete queue;


}

void ControlThread::run()
{
    while (!bStop)
    {
        if (queue->count()>0)
        {
            QString cmd = queue->dequeue();
            proc->execute(cmd);
        }else{
            screenCap();
        }
        msleep(1);
    }

}

void ControlThread::pushCmd(QString cmd)
{
    queue->enqueue(cmd);

}

void ControlThread::screenCap()
{
    proc->execute("adb shell screencap /cache/cap.png");
    proc->execute("adb pull /cache/cap.png c:\\cap.png");
    emit capChanged();
}

void ControlThread::stop()
{
    bStop = true;
}
