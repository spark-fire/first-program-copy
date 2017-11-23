#ifndef SICK_PIMXSENSORPLUGIN_H
#define SICK_PIMXSENSORPLUGIN_H

#include <QObject>
#include "aubo_abstractinterface.h"
#include "cameracontrol.h"

class SICK_PIMxSensorPlugin : public QObject, public AUBO_CameraInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "aubo-robotics.ORPE.Extentions.allen.SICK_PIMxSensorPlugin.1.0.0" FILE "SICK_PIMx.json")
    Q_INTERFACES(AUBO_CameraInterface)
public:
    explicit SICK_PIMxSensorPlugin(QObject *parent = 0);

    QString organization() const Q_DECL_OVERRIDE;
    QString whatsThis() const Q_DECL_OVERRIDE;
    QString name() const Q_DECL_OVERRIDE;
    QString author() const Q_DECL_OVERRIDE;
    QString version() const Q_DECL_OVERRIDE;
    QString toolTip() const Q_DECL_OVERRIDE;
    //QIcon icon() const Q_DECL_OVERRIDE;
    QObject *instance() Q_DECL_OVERRIDE;

    bool readyForWork() Q_DECL_OVERRIDE;
    bool stopPluginDevice() Q_DECL_OVERRIDE;
    bool pausePluginDevice() Q_DECL_OVERRIDE;
    bool continuePluginDevice() Q_DECL_OVERRIDE;

    QWidget *createPluginWidget(QWidget *parent) Q_DECL_OVERRIDE;
    QWidget *createConditionWidget(QWidget *parent) Q_DECL_OVERRIDE;

    void position(const double &compensationX, const double &compensationY,double &pos_x,
                  double &pos_y, const bool istakePhoto)  Q_DECL_OVERRIDE;
    void sortingShape(int &shape) Q_DECL_OVERRIDE;
    void toolOffsetPosition(double &offset_x, double &offset_y,
                            double &offset_z)  Q_DECL_OVERRIDE;
    void robotJointRotateRadian(const int &jointNo, double &jointRadianValue)  Q_DECL_OVERRIDE;

private:

    CameraControl *m_camera;

};

#endif // SICK_PIMXSENSORPLUGIN_H
