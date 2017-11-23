#ifndef AUBO_ABSTRACTINTERFACE_H
#define AUBO_ABSTRACTINTERFACE_H

#include <QObject>
class QWidget;

class AUBO_AbstractInterface
{
public:
    virtual ~AUBO_AbstractInterface() {}

    virtual QString organization() const = 0;
    // return the plugin description
    virtual QString whatsThis() const = 0;
    // return plugin name
    virtual QString name() const = 0;
    // return plugin author
    virtual QString author() const = 0;
    // return plugin version
    virtual QString version() const = 0;
    // reutrn plugin tooltip
    virtual QString toolTip() const = 0;
    // return the icon when the plugin loaded in ORPE
   // virtual QIcon icon() const  = 0;

    virtual QObject *instance() = 0;

    //call this function before parent program start , test the device is ready or not
    virtual bool readyForWork() = 0;
    //stop plugin device by parent
    virtual bool stopPluginDevice() = 0;
    //if the device can bu pasued, call this function to pause device by parent
    virtual bool pausePluginDevice() = 0;

    virtual bool continuePluginDevice() = 0;

    virtual QWidget *createPluginWidget(QWidget *parent) = 0;
    virtual QWidget *createConditionWidget(QWidget *parent) = 0;

};

///
/// \brief The AUBO_GripperInterface class
///
///
class AUBO_GripperInterface: public AUBO_AbstractInterface
{
public:
    virtual ~AUBO_GripperInterface() {}

    virtual int moveGripper(bool block = true)= 0;
    virtual void setGripperParams(const int pos = 0, const int force = 0,
                                  const int speed = 0) = 0;
};


///
/// \brief The AUBO_CameraInterface class
///
///
class AUBO_CameraInterface: public AUBO_AbstractInterface
{
public:
    virtual ~AUBO_CameraInterface() {}

    virtual void position(const double &compensationX, const double &compensationY,
                          double &pos_x, double &pos_y, const bool istakePhoto) = 0;
    virtual void sortingShape(int &shape) = 0;
    virtual void toolOffsetPosition(double &offset_x, double &offset_y,
                                    double &offset_z) = 0;
    virtual void robotJointRotateRadian(const int &jointNo, double &jointRadianValue) = 0;
};

#define AUBO_AbstractInterface_iid "aubo-robotics.ORPE.Extentions.allen.AUBO_AbstractInterface.1.0.0"
Q_DECLARE_INTERFACE(AUBO_AbstractInterface,  AUBO_AbstractInterface_iid)

#define AUBO_GripperInterface_iid "aubo-robotics.ORPE.Extentions.allen.AUBO_GripperInterface.1.0.0"
Q_DECLARE_INTERFACE(AUBO_GripperInterface,  AUBO_GripperInterface_iid)

#define AUBO_CameraInterface_iid "aubo-robotics.ORPE.Extentions.allen.AUBO_CameraInterface.1.0.0"
Q_DECLARE_INTERFACE(AUBO_CameraInterface,  AUBO_CameraInterface_iid)

#endif // AUBO_ABSTRACTINTERFACE_H
