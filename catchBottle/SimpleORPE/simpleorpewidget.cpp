#include "simpleorpewidget.h"
#include "ui_simpleorpewidget.h"
#include <QDir>
#include <QPluginLoader>
#include <QDebug>

SimpleORPEWidget::SimpleORPEWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpleORPEWidget),
    m_wInterface(NULL),
    m_plgWidget(NULL)
{
    ui->setupUi(this);
    connect(ui->pbnExit, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->listWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(slotUpdatePluginsWidget()));

    this->initialWidgets();

}

SimpleORPEWidget::~SimpleORPEWidget()
{
    delete ui;
}

void SimpleORPEWidget::slotUpdatePluginsWidget()
{
    int index = ui->listWidget->currentRow();
    ui->stackedWidget->setCurrentIndex(index);
}

bool SimpleORPEWidget::loadPlugin()
{
    bool ok = false;
    QDir pluginsDir(qApp->applicationDirPath());

#ifdef Q_OS_LINUX
    pluginsDir.cdUp();
#elif  defined (Q_OS_MAC)
    pluginsDir.cdUp();
    pluginsDir.cdUp();
    pluginsDir.cdUp();
    pluginsDir.cdUp();
#elif defined(Q_OS_WIN)
    pluginsDir.cdUp();
    pluginsDir.cdUp();
    pluginsDir.cd("DevicePlugins");
#endif
    pluginsDir.cd("Plugins");

    foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();

        if(plugin) {
//            AUBO_GripperInterface *gripper = qobject_cast<AUBO_GripperInterface *>(plugin);
//            if(gripper) {
//                m_interfaceList.append(gripper);
//                ok = true;
//            }

            AUBO_CameraInterface *other = qobject_cast<AUBO_CameraInterface *>(plugin);
            if(other) {
                m_interfaceList.append(other);
                ok = true;
            }
        }
    }
    return ok;
}

bool SimpleORPEWidget::initialWidgets()
{
    if(this->loadPlugin()){
        for(int i = 0; i < m_interfaceList.count(); i++) {
//            qDebug() << m_interfaceList.count();
            ui->listWidget->insertItem(i, m_interfaceList.at(i)->name());
            QWidget *widget = m_interfaceList.at(i)->createPluginWidget(this);

            if(widget != NULL)
                ui->stackedWidget->insertWidget(i, widget);
        }
        return true;
    }else{

        return false;
    }

}

void SimpleORPEWidget::on_pbn_test_clicked()
{
    double x,y,z;

    m_interfaceList.at(0)->position(0.11,0.22,x,y,true);
    qDebug() << x  << y;

    m_interfaceList.at(0)->robotJointRotateRadian(6,z);
    qDebug() << z;

}
