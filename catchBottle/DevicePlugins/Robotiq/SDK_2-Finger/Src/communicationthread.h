#ifndef COMMUNICATIONTHREAD_H
#define COMMUNICATIONTHREAD_H

#include "portsettings.h"

#include <QThread>
#include <QMutex>

class CommunicationThread : public QThread
{
    Q_OBJECT

public:
    CommunicationThread(QObject *parent = 0);
    ~CommunicationThread();

    void startCommunication(const PortSettings &setting, int waitTimeoutMsec);

protected:
    void run();

signals:
    void reponse(const QByteArray &s);
    void error(const QString &s);
    void timeout(const QString &s);

public slots:
    void slotCurrentRequest(const QByteArray &data);
    void slotStopCommunication();


private:
    QString m_portName;
    QByteArray m_request;
    int m_waitTimeout;
    QMutex m_mutex;
    PortSettings m_setting;
    bool m_quit;

    void clearRequest();
};

#endif // COMMUNICATIONTHREAD_H
