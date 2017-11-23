#ifndef ROBOTIQ_2FINGERPLUGIN_H
#define ROBOTIQ_2FINGERPLUGIN_H

#include <QObject>
#include "aubo_abstractinterface.h"
#include "grippercontrol.h"

class Robotiq_2FingerPlugin : public QObject, public AUBO_GripperInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "aubo-robotics.ORPE.Extentions.allen.Robotiq_2FingerPlugin.1.0.0" FILE "robotiq_2-Finger.json")
    Q_INTERFACES(AUBO_GripperInterface)
public:
    explicit Robotiq_2FingerPlugin(QObject *parent = 0);
    ~Robotiq_2FingerPlugin();

    QString organization() const Q_DECL_OVERRIDE;
    QString whatsThis() const Q_DECL_OVERRIDE;
    QString name() const Q_DECL_OVERRIDE;
    QString author() const Q_DECL_OVERRIDE;
    QString version() const Q_DECL_OVERRIDE;
    QString toolTip() const Q_DECL_OVERRIDE;
    //QIcon icon() const Q_DECL_OVERRIDE;
    QObject *instance() Q_DECL_OVERRIDE;

    QWidget *createPluginWidget(QWidget *parent) Q_DECL_OVERRIDE;
    QWidget *createConditionWidget(QWidget *parent) Q_DECL_OVERRIDE;

    int moveGripper(bool block)Q_DECL_OVERRIDE;
    void setGripperParams(const int pos = 0, const int force = 0,
                          const int speed = 0)  Q_DECL_OVERRIDE;

private:
    GripperControl *m_gripper;
};

#endif // ROBOTIQ_2FINGERPLUGIN_H
