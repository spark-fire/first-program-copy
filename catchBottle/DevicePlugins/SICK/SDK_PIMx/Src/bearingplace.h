#ifndef BEARINGPLACE_H
#define BEARINGPLACE_H

#include <QObject>

class BearingPlace : public QObject
{
    Q_OBJECT
public:
    explicit BearingPlace(QObject *parent = 0);

    void SetWaypoint_x(double point_x);
    void SetWaypoint_y(double point_y);
    void SetWaypoint_z(double point_z);

    double GetWaypoint_x();
    double GetWaypoint_y();
    double GetWaypoint_z();

signals:

public slots:

private:
    double m_waypoint_x;
    double m_waypoint_y;
    double m_waypoint_z;
};

#endif // BEARINGPLACE_H
