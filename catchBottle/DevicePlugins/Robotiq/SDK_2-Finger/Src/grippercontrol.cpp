#include "grippercontrol.h"

GripperControl::GripperControl(QObject *parent) :
    QObject(parent)
{

    connect(&m_dataThread, SIGNAL(sendRequest(QByteArray)), &m_commThread, SLOT(slotCurrentRequest(QByteArray)));
    connect(&m_commThread, SIGNAL(reponse(QByteArray)), &m_dataThread, SLOT(slotReciveRespone(QByteArray)));
    connect(&m_commThread, SIGNAL(error(QString)), this, SLOT(slotHandleError(QString)));
    connect(&m_commThread, SIGNAL(timeout(QString)), this, SLOT(slotHandleTimeout(QString)));

    connect(&m_dataThread, SIGNAL(status(QString)), this , SLOT(slotGripperStatus(QString)));
    connect(&m_dataThread, SIGNAL(error(QString)), this, SLOT(slotHandleError(QString)));

    m_dataThread.startDataThread(m_currentSettings, m_currentStatus);

}

GripperControl::~GripperControl()
{
    m_commThread.slotStopCommunication();
}

void GripperControl::gripperConnect()
{
    m_commThread.startCommunication(m_portSettings, 2000);
}

void GripperControl::gripperDisconnect()
{
    m_commThread.slotStopCommunication();
}

int GripperControl::gripperClear()
{
    return this->m_dataThread.rq_clear();
}

int GripperControl::gripperActive(bool block)
{
    return this->m_dataThread.rq_active(block);
}

int GripperControl::gripperClose(bool block)
{
    return this->m_dataThread.rq_close(m_currentSettings, block);
}

int GripperControl::gripperOpen(bool block)
{
    return this->m_dataThread.rq_open(m_currentSettings, block);
}

int GripperControl::gripperMove(bool block)
{
    return this->m_dataThread.rq_move(m_currentSettings, block);
}

void GripperControl::gripperSetForce(const int &force)
{
    m_currentSettings.setForce(force);
}

void GripperControl::gripperSetSpeed(const int &speed)
{
    m_currentSettings.setSpeed(speed);
}

void GripperControl::gripperSetPosition(const int &position)
{
    m_currentSettings.setPosition(position);
}

void GripperControl::gripperSetParams(const int &pos, const int &speed, const int &force)
{
    this->gripperSetForce(force);
    this->gripperSetPosition(pos);
    this->gripperSetSpeed(speed);
}

void GripperControl::slotGripperStatus(const QString &data)
{
    //Q_UNUSED(data)
    qDebug() << "Gripper Status: " << data;

}

void GripperControl::slotHandleError(const QString &s)
{
//    Q_UNUSED(s)
    qDebug() << "Gripper Errors: " << s;
}

void GripperControl::slotHandleTimeout(const QString &s)
{
    Q_UNUSED(s)
    //qDebug() << "Gripper Timeout: " << s;
}
