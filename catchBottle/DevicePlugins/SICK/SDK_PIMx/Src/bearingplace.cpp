#include "bearingplace.h"

BearingPlace::BearingPlace(QObject *parent) : QObject(parent)
{

}

void BearingPlace::SetWaypoint_x(double point_x)
{
    m_waypoint_x = point_x;
}

void BearingPlace::SetWaypoint_y(double point_y)
{
    m_waypoint_y = point_y;
}

void BearingPlace::SetWaypoint_z(double point_z)
{
    m_waypoint_z = point_z;
}

double BearingPlace::GetWaypoint_x()
{
    return m_waypoint_x;
}

double BearingPlace::GetWaypoint_y()
{
    return m_waypoint_y;
}

double BearingPlace::GetWaypoint_z()
{
    return m_waypoint_z;
}
