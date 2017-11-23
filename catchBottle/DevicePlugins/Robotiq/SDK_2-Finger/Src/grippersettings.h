#ifndef GRIPPERSETTINGS_H
#define GRIPPERSETTINGS_H

#include <QtGlobal>

namespace Gripper
{

// Gripper id
const quint8 g_ID = 0x9;

// Register Address
const quint16 g_regInput = 0x03E8;
const quint16 g_regOutput = 0x07D0;
const quint8 g_regCount = 0x03;

// Function code
const quint8 g_cmdReadHoldingReg = 0x03;
const quint8 g_cmdPresetMultiReg = 0x10;
//const quint8 g_cmdReadWriteMultiReg = 0x17;

}

class GripperSettings
{
public:

    GripperSettings(quint8 pos = 0, quint8 force = 0, quint8 speed = 0, quint8 action = 0)
    {
        m_position = pos;
        m_force = force;
        m_speed = speed;
        m_action = action;
    }

    int position() const {return m_position;}
    void setPosition(const int data) {m_position = static_cast<quint8>(data);}

    int force() const {return m_force;}
    void setForce(const int data) {m_force = static_cast<quint8>(data);}

    int speed() const {return m_speed;}
    void setSpeed(const int data) { m_speed = static_cast<quint8>(data);}

    quint8 action() const {return m_action;}
    void setAction(const quint8 data) { m_action = data;}


private:
    quint8 m_position;
    quint8 m_force;
    quint8 m_speed;
    quint8 m_action;
};

class GripperStatus
{

public:
    GripperStatus():m_status(0),m_fault(0),m_posRequest(0),m_posAcutal(0),m_current(0){}

    void setStatus(const quint8 data) { m_status = data;}
    quint8 status() const {return m_status;}

    void setFault(const quint8 data) { m_fault = data;}
    quint8 fault() const {return m_fault;}

    void setPosRequest(const quint8 data) { m_posRequest = data;}
    quint8 posRequest() const {return m_posRequest;}

    void setPosAcutal(const quint8 data) { m_posAcutal = data;}
    quint8 posAcutal() const {return m_posAcutal;}

    void setCurrent(const quint8 data) { m_current = data;}
    quint8 current() const {return m_current;}

private:
    quint8 m_status;
    quint8 m_fault;
    quint8 m_posRequest;
    quint8 m_posAcutal;
    quint8 m_current;
};



#endif // GRIPPERSETTINGS_H
