#ifndef PORTSETTINGS_H
#define PORTSETTINGS_H

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort>

class PortSettings
{
public:
    const QString &portName() const {return m_name;}
    void setPortName(const QString name) {m_name = name;}

    const QSerialPort::BaudRate &baudRate() const {return m_rate;}
    void setBaudRate(QSerialPort::BaudRate baudrate){m_rate = baudrate;}

    const QSerialPort::DataBits &dataBits() const {return m_bits;}
    void setDataBits(QSerialPort::DataBits bits){m_bits = bits;}

    const QSerialPort::Parity &parity() const {return m_parity;}
    void setParity(QSerialPort::Parity parity){m_parity = parity;}

    const QSerialPort::StopBits &stopBits() const {return m_stopBits;}
    void setStopBits(QSerialPort::StopBits bits){m_stopBits = bits;}

    const QSerialPort::FlowControl &flowControl() const {return m_flowControl;}
    void setFlowControl(QSerialPort::FlowControl flowcontrol){m_flowControl = flowcontrol;}


private:
    QString m_name;
    QSerialPort::BaudRate m_rate;
    QSerialPort::DataBits m_bits;
    QSerialPort::Parity m_parity;
    QSerialPort::StopBits m_stopBits;
    QSerialPort::FlowControl m_flowControl;

};

#endif // PORTSETTINGS_H
