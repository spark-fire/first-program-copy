#include "robotiq_2fingerplugin.h"
#include "robotiq_2fingerpluginwidget.h"

Robotiq_2FingerPlugin::Robotiq_2FingerPlugin(QObject *parent)
    : QObject(parent),
      m_gripper(new GripperControl)
{

}

Robotiq_2FingerPlugin::~Robotiq_2FingerPlugin()
{
    if(m_gripper != NULL)
        delete m_gripper;
}

QString Robotiq_2FingerPlugin::organization() const
{
    return QString("Robotiq Inc.");
}

QString Robotiq_2FingerPlugin::whatsThis() const
{
    return QString("This plugin used for control Robotiq 2-Finger Gripper. ");
}

QString Robotiq_2FingerPlugin::name() const
{
    return QString("2-Finger Gripper");
}

QString Robotiq_2FingerPlugin::author() const
{
    return QString("Allen Zhang <zxt@aubo-robotics.cn>");
}

QString Robotiq_2FingerPlugin::version() const
{
    return QString("1.0.0");
}

QString Robotiq_2FingerPlugin::toolTip() const
{
    return QString("This is Robotiq Gripper control plugin.");
}

QObject *Robotiq_2FingerPlugin::instance()
{
    return m_gripper;
}

//QIcon Robotiq_2FingerPlugin::icon() const
//{

//}

QWidget *Robotiq_2FingerPlugin::createPluginWidget(QWidget *parent)
{
    if (m_gripper != NULL)
        return new Robotiq_2FingerPluginWidget(m_gripper, parent);
    else
        return NULL;
}

QWidget *Robotiq_2FingerPlugin::createConditionWidget(QWidget *parent)
{
    Q_UNUSED(parent)

    return NULL;
}

int Robotiq_2FingerPlugin::moveGripper(bool block)
{
    return m_gripper->gripperMove(block);
}

void Robotiq_2FingerPlugin::setGripperParams(const int pos, const int force, const int speed)
{
    m_gripper->gripperSetParams(pos, speed, force);
}
