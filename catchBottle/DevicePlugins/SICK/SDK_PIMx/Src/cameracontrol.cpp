#include "cameracontrol.h"
#include <QFile>
#include <QDebug>

CameraControl::CameraControl(QObject *parent)
    : QObject(parent)
{
    connect(&m_thread, &ProcessThread::cameraResult, this, &CameraControl::camera_data);
    connect(&m_thread, &ProcessThread::cameraGetPositionX, this, &CameraControl::camera_position_x);
    connect(&m_thread, &ProcessThread::cameraGetPositionY, this, &CameraControl::camera_position_y);
    connect(&m_thread, &ProcessThread::cameraCharacter, this, &CameraControl::camera_character);
}

CameraControl::~CameraControl()
{

}

void CameraControl::setCameraAddress(const QString &addr)
{
    m_address = addr;
    this->m_thread.initialCamera(m_address, NULL);
}

int CameraControl::cam_position(QString &pos_x, QString &pos_y,
                                QString &pos_z)
{
    this->m_thread.initialCamera(m_address, NULL);
    return this->m_thread.res_position(pos_x, pos_y, pos_z, true);
}

void CameraControl::cam_ready_point(QString &ready_point_x, QString &ready_point_y)
{
    this->m_thread.readyPoint(ready_point_x,ready_point_y);
}

void CameraControl::setToolOffset(const QString &offset_x, const QString &offset_y,
                                  const QString &offset_z)
{
    m_toolOffsetx = offset_x;
    m_toolOffsety = offset_y;
    m_toolOffsetz = offset_z;
}

void CameraControl::getToolOffset( QString &getOffset_x,  QString &getOffset_y,
                                   QString &getOffset_z)
{
    getOffset_x = m_toolOffsetx;
    getOffset_y = m_toolOffsety;
    getOffset_z = m_toolOffsetz;
}

void CameraControl::setCoordinateCompensation(const QString &compensation_x,
                                              const QString &compensation_y,
                                              const QString &compensation_z)
{
    m_xCompensation = compensation_x;
    m_yCompensation = compensation_y;
    m_zCompensation = compensation_z;
}

void CameraControl::getCoordinateCompensation(QString &getcompensation_x,
                                              QString &getcompensation_y,
                                              QString &getcompensation_z)
{
    getcompensation_x = m_xCompensation;
    getcompensation_y = m_yCompensation;
    getcompensation_z = m_zCompensation;
}

void CameraControl::getSICKCharacter(QMap<QString, QString > &recCharacter)
{
    m_thread.recSickCharacter(recCharacter);
}

void CameraControl::initCameraObject()
{
    m_thread.m_dataFilter = 1.0;
}

void CameraControl::getObjectShape(QString &shape)
{
    shape = m_thread.objectShape;
}

void CameraControl::cam_joint_radian(double &joint_radian)
{
    joint_radian = m_thread.m_jointRadian.toDouble();
}



































