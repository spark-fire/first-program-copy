#include "communicationthread.h"
#include "portsettings.h"

#include <QDebug>

CommunicationThread::CommunicationThread(QObject *parent)
    : QThread(parent),
      m_waitTimeout(0),
      m_quit(false)
{

}

CommunicationThread::~CommunicationThread()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();
    //wait for the thread has finished.
    wait();
}

void CommunicationThread::startCommunication(const PortSettings &setting, int waitTimeoutMsec)
{
    QMutexLocker locker(&m_mutex);
    this->m_setting = setting;
    this->m_waitTimeout = waitTimeoutMsec;
    this->m_quit = false;

    if(!isRunning()){
        start();
    }else{
       qDebug() << "Gripper is already active.";
    }

}

void CommunicationThread::run()
{
    bool currentPortNameChanged = false;

    m_mutex.lock();
    PortSettings settings = m_setting;
    QString currentPortName;
    if (currentPortName != settings.portName()) {
        currentPortName = settings.portName();
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = m_waitTimeout;
    QByteArray currentRequest = m_request;
    m_mutex.unlock();

    QSerialPort serial;

    while (!m_quit) {

        if (currentPortNameChanged) {
            serial.close();
            serial.setPortName(currentPortName);
            serial.setBaudRate(settings.baudRate());
            serial.setDataBits(settings.dataBits());
            serial.setParity(settings.parity());
            serial.setStopBits(settings.stopBits());
            serial.setFlowControl(settings.flowControl());

            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(currentPortName).arg(serial.error()));
                return;
            }
        }

        if(!currentRequest.isEmpty()){

//            emit this->reponse(currentRequest);

            QByteArray requestData = currentRequest;

            serial.write(requestData);
            if (serial.waitForBytesWritten(m_waitTimeout)) {

                // read response
                if (serial.waitForReadyRead(currentWaitTimeout)) {
                    QByteArray responseData = serial.readAll();
                    while (serial.waitForReadyRead(10))
                        responseData += serial.readAll();

                    emit this->reponse(responseData);
                    this->clearRequest();

                } else {
                    emit timeout(tr("Serial:: Wait read response timeout %1")
                                 .arg(QTime::currentTime().toString()));
                }

            } else {
                emit timeout(tr("Serial:: Wait write request timeout %1")
                             .arg(QTime::currentTime().toString()));
            }
        }

        m_mutex.lock();
        if (currentPortName != settings.portName()) {
            currentPortName = settings.portName();
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = m_waitTimeout;
        currentRequest = m_request;
        m_mutex.unlock();
    }

}

void CommunicationThread::slotCurrentRequest(const QByteArray &data)
{
      m_mutex.lock();
      m_request = data;
      m_mutex.unlock();
}

void CommunicationThread::slotStopCommunication()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();
}

void CommunicationThread::clearRequest()
{
    m_mutex.lock();
    m_request.clear();
    m_mutex.unlock();
}

