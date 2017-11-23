#include "sick_pimxsensorplugin.h"
#include "sick_pimxpluginwidget.h"
#include <QDebug>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QTextStream>

SICK_PIMxSensorPlugin::SICK_PIMxSensorPlugin(QObject *parent)
    : QObject(parent),
      m_camera(new CameraControl)
{


}

QString SICK_PIMxSensorPlugin::organization() const
{
    return QString("SICK Inc.");
}

QString SICK_PIMxSensorPlugin::whatsThis() const
{
    return QString("This is SICK inspector PIM-series sensor plugin.");
}

QString SICK_PIMxSensorPlugin::name() const
{
    return QString("Inspector PIM-Series");
}

QString SICK_PIMxSensorPlugin::author() const
{
    return QString("Allen Zhang <zxt@aubo-robotics.cn>");
}

QString SICK_PIMxSensorPlugin::version() const
{
    return QString("1.0.1");
}

QString SICK_PIMxSensorPlugin::toolTip() const
{
    return QString("This is SICK inspector PIM-series sensor plugin.");
}

QObject *SICK_PIMxSensorPlugin::instance()
{
    return m_camera;
}

bool SICK_PIMxSensorPlugin::readyForWork()
{
    return true;
}

bool SICK_PIMxSensorPlugin::stopPluginDevice()
{
    return true;
}

bool SICK_PIMxSensorPlugin::pausePluginDevice()
{
    return true;
}

bool SICK_PIMxSensorPlugin::continuePluginDevice()
{
    return true;
}

//QIcon Robotiq_2FingerPlugin::icon() const
//{

//}

QWidget *SICK_PIMxSensorPlugin::createPluginWidget(QWidget *parent)
{
    if(m_camera != NULL)
    {
        SICK_PIMxPluginWidget *m_pluginWgt = new SICK_PIMxPluginWidget(m_camera, parent);
        m_pluginWgt->setPluginInfomation(organization(),whatsThis(),name(),author(),version());
        return m_pluginWgt/*new SICK_PIMxPluginWidget(m_camera, parent)*/;
    }
    else
        return NULL;
}

QWidget *SICK_PIMxSensorPlugin::createConditionWidget(QWidget *parent)
{
    Q_UNUSED(parent)
    return NULL;
}

void SICK_PIMxSensorPlugin::position(const double &compensationX, const double &compensationY,double &pos_x,
                                     double &pos_y,const bool istakePhoto)
{
    QString x,y,z;
    QString compensation_x,compensation_y,compensation_z;

    if(true == istakePhoto){

        m_camera->cam_position(x, y, z);

    }else{

        m_camera->cam_ready_point(x,y);

    }

    m_camera->getCoordinateCompensation(compensation_x, compensation_y,
                                        compensation_z);

    pos_x = (x.toDouble())/1000 + (compensation_x.toDouble())/1000 + compensationX;
    pos_y = (y.toDouble())/1000 + (compensation_y.toDouble())/1000 + compensationY;
}

void SICK_PIMxSensorPlugin::sortingShape(int &shape)
{
    QTime   m_time = QTime::currentTime();
    QString mm_time = m_time.toString();

    QFile   file("/root/CameraShapelog.txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
    {
        qDebug() << "Open failed" << endl;
    }
    QTextStream out(&file);

    QString str = "**********begin_shape***********";
    out<<"["<<mm_time<<"]"<<":"<<str<<endl;

    QString objectShape;
    str = "begin_not_take_photo";
    out<<"["<<mm_time<<"]"<<":"<<str<<endl;
    m_camera->getObjectShape(objectShape);
    str = "finish_not_take_photo";
    out<<"["<<mm_time<<"]"<<":"<<str<<endl;
    shape = objectShape.toInt();
    str = "**********finish***********";
    out<<"["<<mm_time<<"]"<<":"<<str<<endl;
}

void SICK_PIMxSensorPlugin::toolOffsetPosition(double &offset_x, double &offset_y,
                        double &offset_z)
{
    QString toolOffsetx,toolOffsety,toolOffsetz;
    m_camera->getToolOffset(toolOffsetx,toolOffsety,toolOffsetz);

    offset_x = toolOffsetx.toDouble();
    offset_y = toolOffsety.toDouble();
    offset_z = toolOffsetz.toDouble();
}

void SICK_PIMxSensorPlugin::robotJointRotateRadian(const int &jointNo, double &jointRadianValue)
{
    double jointRadian;
    switch (jointNo) {
    case 1:
        jointRadianValue = -0.000003;
        break;
    case 2:
        jointRadianValue = -0.127267;
        break;
    case 3:
        jointRadianValue = -1.321122;
        break;
    case 4:
        jointRadianValue = 0.376934;
        break;
    case 5:
        jointRadianValue = -1.570796;
        break;
    case 6:
         m_camera->cam_joint_radian(jointRadian);
         jointRadianValue = jointRadian;
         qDebug() << "jointRadianValue" << jointRadianValue;
        break;
    default:
        break;
    }
}



























