#include "mntaskthread.h"
#include <QtConcurrent/QtConcurrent>

Mntaskthread* Mntaskthread::initance()
{
    static Mntaskthread p;
    return &p;
}

Mntaskthread::Mntaskthread(QObject *parent) : QObject(parent)
{
    threadPool.setMaxThreadCount(4);
    stop = false;
}

void Mntaskthread::insertTask(QRunnable* qrTask)
{
    QMutexLocker locker(&qMutex);
    qDque.append(qrTask);
}

int Mntaskthread::work(Mntaskthread *mmtt)
{
    while(!mmtt->stop)
    {
        if (!mmtt->qDque.empty())
        {            
            QRunnable* ptrRunner = NULL;

            {
                QMutexLocker locker(&mmtt->qMutex);
                ptrRunner = mmtt->qDque.first();
                mmtt->qDque.erase(mmtt->qDque.begin());
                qDebug()<< "Dque Size: "<<mmtt->qDque.size();
            }

            if (ptrRunner != NULL)
            {
                //delete ptrRunner;
                mmtt->threadPool.start(ptrRunner);
            }

        }
        else
        {
            QThread::msleep(1000);
        }
    }
    mmtt->threadPool.clear();
    return 0;
}

int Mntaskthread::startTaskThread()
{
    stop = false;
    futrue = QtConcurrent::run(work, this);
    return 0;
}

int Mntaskthread::stopTaskThread()
{
    stop = true;
    threadPool.waitForDone();
    return 0;
}
