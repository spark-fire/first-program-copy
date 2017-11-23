#ifndef GRIPPERCONTROL_H
#define GRIPPERCONTROL_H

#include "dataprocessthread.h"
#include "communicationthread.h"
#include "portsettings.h"

#include <QObject>

class GripperControl : public QObject
{
    Q_OBJECT
public:
    explicit GripperControl(QObject *parent = 0);
    ~GripperControl();

    void gripperConnect();
    void gripperDisconnect();

    int gripperClear();
    ///
    /// \brief gripperActive(bool block = true)
    ///   Sends the Gripper activation command. If it is already activated, nothing happens. Note that the
    ///   Gripper must be activated to complete any other operation.If block == ture, Program execution waits
    ///   for the activation.
    ///
    int gripperActive(bool block = true);

    int gripperClose(bool block = true);

    int gripperOpen(bool block =true);

    int gripperMove(bool block = true);

    void gripperRest(bool block = true);

    void gripperSetForce(const int &force);

    void gripperSetSpeed(const int &speed);

    void gripperSetPosition(const int &position);

    void gripperSetParams(const int &pos, const int&speed, const int &force);

    GripperSettings &gripperSettings() { return m_currentSettings; }
    PortSettings &portSettings() {return m_portSettings;}
    const GripperStatus &gripperStauts() const {return m_currentStatus;}

private slots:
    void slotGripperStatus(const QString &data);
    void slotHandleError(const QString &s);
    void slotHandleTimeout(const QString &s);

private:

    GripperSettings m_currentSettings;
    GripperStatus m_currentStatus;
    PortSettings m_portSettings;

    DataProcessThread m_dataThread;
    CommunicationThread m_commThread;
};

#endif // GRIPPERCONTROL_H
