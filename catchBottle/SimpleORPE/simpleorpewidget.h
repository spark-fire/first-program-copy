#ifndef SIMPLEORPEWIDGET_H
#define SIMPLEORPEWIDGET_H

#include <QWidget>
#include <QVector>

#include "aubo_abstractinterface.h"

namespace Ui {
class SimpleORPEWidget;
}

class SimpleORPEWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleORPEWidget(QWidget *parent = 0);
    ~SimpleORPEWidget();

private slots:
    void slotUpdatePluginsWidget();

    void on_pbn_test_clicked();

private:
    Ui::SimpleORPEWidget *ui;

    bool loadPlugin();
    bool initialWidgets();
    AUBO_GripperInterface *m_wInterface;
    //QVector<AUBO_AbstractInterface * > m_interfaceList;
    QVector<AUBO_CameraInterface * > m_interfaceList;
    QWidget *m_plgWidget;

};

#endif // SIMPLEORPEWIDGET_H
