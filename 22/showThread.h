#ifndef SHOWTHREAD_H
#define SHOWTHREAD_H

#include <QThread>

class showThread : public QThread
{
    Q_OBJECT
public:
    explicit showThread(QObject *parent = 0);
    void stop();
    int key;
    void QAM_Diagram();

protected:
    void run();

private:
    volatile bool stopped;

signals:

public slots:

};

#endif // SHOWTHREAD_H
