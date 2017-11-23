#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

#include "bearingplace.h"
#include "processthread.h"
#include <QObject>

class CameraControl : public QObject
{
    Q_OBJECT
public:
    explicit CameraControl(QObject *parent = 0);
    ~CameraControl();

    void setCameraAddress(const QString &addr);
    void setToolOffset(const QString &offset_x, const QString &offset_y,
                       const QString &offset_z);
    const QString &cameraAddress() const {return m_address;}

    int cam_position(QString &pos_x, QString &pos_y,
                     QString &pos_z);
    void cam_ready_point(QString &ready_point_x, QString &ready_point_y);
    void getToolOffset( QString &getOffset_x,  QString &getOffset_y,
                                       QString &getOffset_z);
    void getSICKCharacter(QMap<QString, QString > &recCharacter);

    void setCoordinateCompensation(const QString &compensation_x,
                                   const QString &compensation_y,
                                   const QString &compensation_z);
    void getCoordinateCompensation(QString &getcompensation_x,
                                   QString &getcompensation_y,
                                   QString &getcompensation_z);
    void initCameraObject();
    void getObjectShape(QString &shape);
    void cam_joint_radian(double &joint_radian);

private:


signals:
    void camera_data(QString);
    void camera_position_x(QString);
    void camera_position_y(QString);
    void camera_character(QMap<QString, QString>);
public slots:

private:
    ProcessThread m_thread;
    QString m_address;

    QString m_toolOffsetx;
    QString m_toolOffsety;
    QString m_toolOffsetz;

    QString m_xCompensation;
    QString m_yCompensation;
    QString m_zCompensation;
};

#endif // CAMERACONTROL_H



































