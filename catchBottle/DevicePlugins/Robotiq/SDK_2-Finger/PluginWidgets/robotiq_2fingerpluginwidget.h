#ifndef ROBOTIQ_2FINGERPLUGINWIDGET_H
#define ROBOTIQ_2FINGERPLUGINWIDGET_H

#include "portsettings.h"
#include "communicationthread.h"
#include "grippercontrol.h"

#include <QWidget>

namespace Ui {
class Robotiq_2FingerPluginWidget;
}


class Robotiq_2FingerPluginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit Robotiq_2FingerPluginWidget(GripperControl *g, QWidget *parent = 0);
    ~Robotiq_2FingerPluginWidget();


signals:

private slots:
    // slots for settings page
    void on_pbn_activeGripper_clicked();
    void on_pbn_disableGripper_clicked();
    void on_pbn_connect_clicked();

    // slots for gripper
    void slotHandleError(const QString &data);

    // slots for control page
    void on_pbn_gripperClose_clicked();
    void on_pbn_gripperOpen_clicked();
    void slotUpdateGripperParam();
    void on_pbn_gripperAction_clicked();


private:

    void initialSettingsPage();
    void initialControlPage();
    void initialAboutPages();

    ///
    /// \brief Functions for Settings page
    ///
    void fillPortsParameters();
    void fillPortsInfo();
    void updatePortSettings();

    ///
    /// \brief Functions for About page
    ///


    ///
    /// \brief Functions for Control page
    ///



    Ui::Robotiq_2FingerPluginWidget *ui;

    GripperControl *m_gripper;

};

#endif // ROBOTIQ_2FINGERPLUGINWIDGET_H
