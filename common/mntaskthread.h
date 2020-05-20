#ifndef MNTASKTHREAD_H
#define MNTASKTHREAD_H

#include <QObject>
#include <QList>
#include <QEventLoop>
#include <QFuture>
#include "mnsshtask.h"

typedef QList<QRunnable*> QTaskDeque;

class Mntaskthread : public QObject
{
    Q_OBJECT
public:
    static Mntaskthread* initance();
    int startTaskThread();
    int stopTaskThread();
    void insertTask(QRunnable* qrTask);
signals:

public slots:

private:
    static int work(Mntaskthread *mmtt);
    explicit Mntaskthread(QObject *parent = nullptr);
public:
    QThreadPool threadPool;
    QFuture<void> futrue;
    QTaskDeque qDque;
    bool stop;
    QMutex qMutex;
    static Mntaskthread p;
};

#endif // MNTASKTHREAD_H
