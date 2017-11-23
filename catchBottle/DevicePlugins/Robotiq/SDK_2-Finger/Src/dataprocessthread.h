#ifndef DATAPROCESSTHREAD_H
#define DATAPROCESSTHREAD_H

#include "grippersettings.h"
#include <QThread>
#include <QQueue>
#include <QMutex>
//#include <QWaitCondition>
#include <QEventLoop>


enum GripperCommand{

    GRIPPER_CLEAR = 0x0,
    GRIPPER_ACTIIVE,
    GRIPPER_OPEN,
    GRIPPER_CLOSE,
    GRIPPER_MOVE
};


class DataProcessThread : public QThread
{
    Q_OBJECT

public:
    explicit DataProcessThread(QObject *parent = 0);
    ~DataProcessThread();

    void startDataThread(GripperSettings &settings, GripperStatus &status);

    int rq_clear();
    int rq_active(bool wait = true);
    int rq_open(GripperSettings settings, bool wait = true);
    int rq_close(GripperSettings settings, bool wait = true);
    int rq_move(GripperSettings settings, bool wait = true);

signals:
    void sendRequest(const QByteArray &s);
    void error(const QString &s);
    void status(const QString &s);

protected:
    void run();

public slots:
    void slotReciveRespone(const QByteArray &data);

private:

    void writeRequestHeader(QByteArray &req);
    void readRequestHeader(QByteArray &req);

    void processWriteRequest(GripperCommand cmd);
    void processReadRequest();
    int rq_actions(int idx, bool isWait);

    QByteArray m_theLastWriteRequest;
    QByteArray m_theLastReadRequest;
    QByteArray m_currentRequest;

    volatile bool m_isCurrentCmdResponse;
    volatile bool m_waitMode;
    bool m_quit;

    QQueue<QPair<int, bool> > m_queue;
    QMutex m_mutex;
//    QWaitCondition  m_waitActionComplete;

    GripperSettings m_settings;
    GripperStatus m_status;

//    bool m_ok ;
    QEventLoop m_eventloop;

};

#endif // DATAPROCESSTHREAD_H
