#include "robotiq_2fingerpluginwidget.h"
#include "ui_robotiq_2fingerpluginwidget.h"


#include <QDebug>

static const char blankString[] = QT_TRANSLATE_NOOP("GripperSettingsPage", "N/A");

Robotiq_2FingerPluginWidget::Robotiq_2FingerPluginWidget(GripperControl *g, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Robotiq_2FingerPluginWidget),
    m_gripper(g)

{
    ui->setupUi(this);

    this->initialControlPage();
    this->initialSettingsPage();
    this->initialAboutPages();

}

Robotiq_2FingerPluginWidget::~Robotiq_2FingerPluginWidget()
{
    delete ui;
}

void Robotiq_2FingerPluginWidget::initialSettingsPage()
{
    fillPortsInfo();
    fillPortsParameters();
    updatePortSettings();
}

void Robotiq_2FingerPluginWidget::initialControlPage()
{
    ui->slider_force->setValue(m_gripper->gripperSettings().force());
    ui->slider_position->setValue(m_gripper->gripperSettings().position());
    ui->slider_speed->setValue(m_gripper->gripperSettings().speed());

    connect(ui->slider_position, SIGNAL(valueChanged(int)), this ,SLOT(slotUpdateGripperParam()));
    connect(ui->slider_force, SIGNAL(valueChanged(int)), this ,SLOT(slotUpdateGripperParam()));
    connect(ui->slider_speed, SIGNAL(valueChanged(int)), this ,SLOT(slotUpdateGripperParam()));
    this->slotUpdateGripperParam();
}

void Robotiq_2FingerPluginWidget::initialAboutPages()
{

}

void Robotiq_2FingerPluginWidget::fillPortsParameters()
{
    ui->cbx_baudrate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->cbx_baudrate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->cbx_baudrate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->cbx_baudrate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->cbx_baudrate->addItem(tr("Custom"));
    ui->cbx_baudrate->setCurrentIndex(3);

    ui->cbx_dataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->cbx_dataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->cbx_dataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->cbx_dataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->cbx_dataBits->setCurrentIndex(3);

    ui->cbx_parity->addItem(tr("None"), QSerialPort::NoParity);
    ui->cbx_parity->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->cbx_parity->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->cbx_parity->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->cbx_parity->addItem(tr("Space"), QSerialPort::SpaceParity);
    ui->cbx_parity->setCurrentIndex(1);

    ui->cbx_stopBits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->cbx_stopBits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
    ui->cbx_stopBits->setCurrentIndex(0);

    ui->cbx_flowControl->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->cbx_flowControl->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->cbx_flowControl->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
    ui->cbx_flowControl->setCurrentIndex(0);
}

void Robotiq_2FingerPluginWidget::fillPortsInfo()
{
    ui->cbx_port->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
#ifdef Q_OS_LINUX
        list << QString("/dev/").append(info.portName())
#elif defined(Q_OS_WIN)
        list << info.portName()
#endif
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->cbx_port->addItem(list.first(), list);
    }

    ui->cbx_port->addItem(tr("Custom"));
}

void Robotiq_2FingerPluginWidget::updatePortSettings()
{

    m_gripper->portSettings().setPortName(ui->cbx_port->currentText());
    m_gripper->portSettings().setBaudRate(static_cast<QSerialPort::BaudRate>(ui->cbx_baudrate->itemData(
                                                                       ui->cbx_baudrate->currentIndex()).toInt()));
    m_gripper->portSettings().setDataBits(static_cast<QSerialPort::DataBits>(ui->cbx_dataBits->itemData(
                                                                       ui->cbx_dataBits->currentIndex()).toInt()));
    m_gripper->portSettings().setStopBits(static_cast<QSerialPort::StopBits>(ui->cbx_stopBits->itemData(
                                                                       ui->cbx_stopBits->currentIndex()).toInt()));
    m_gripper->portSettings().setParity(static_cast<QSerialPort::Parity>(ui->cbx_parity->itemData(
                                                                       ui->cbx_parity->currentIndex()).toInt()));
    m_gripper->portSettings().setFlowControl(static_cast<QSerialPort::FlowControl>(ui->cbx_flowControl->itemData(
                                                                       ui->cbx_flowControl->currentIndex()).toInt()));


//    qDebug() << m_gripper->portSettings().portName() << endl
//             << m_gripper->portSettings().baudRate() << endl
//             << m_gripper->portSettings().dataBits() << endl
//             << m_gripper->portSettings().stopBits() << endl
//             << m_gripper->portSettings().parity() << endl
//             << m_gripper->portSettings().flowControl() << endl;
}

void Robotiq_2FingerPluginWidget::on_pbn_activeGripper_clicked()
{
    m_gripper->gripperClear();
    m_gripper->gripperActive();
}

void Robotiq_2FingerPluginWidget::slotHandleError(const QString &data)
{
    qDebug() << data;
}

void Robotiq_2FingerPluginWidget::on_pbn_gripperClose_clicked()
{
    int res = m_gripper->gripperClose();
    qDebug() << res;
}

void Robotiq_2FingerPluginWidget::on_pbn_gripperOpen_clicked()
{

    int res = m_gripper->gripperOpen();
    qDebug() << res;
}

void Robotiq_2FingerPluginWidget::slotUpdateGripperParam()
{
    m_gripper->gripperSettings().setPosition(ui->slider_position->value());
    m_gripper->gripperSettings().setForce(ui->slider_force->value());
    m_gripper->gripperSettings().setSpeed(ui->slider_speed->value());

    ui->le_position->setText(QString::number(m_gripper->gripperSettings().position()));
    ui->le_force->setText(QString::number(m_gripper->gripperSettings().force()));
    ui->le_speed->setText(QString::number(m_gripper->gripperSettings().speed()));
}

void Robotiq_2FingerPluginWidget::on_pbn_disableGripper_clicked()
{
    m_gripper->gripperDisconnect();
}

void Robotiq_2FingerPluginWidget::on_pbn_connect_clicked()
{
    updatePortSettings();
    m_gripper->gripperConnect();

}

void Robotiq_2FingerPluginWidget::on_pbn_gripperAction_clicked()
{
    int res = m_gripper->gripperMove();
    qDebug() << res;
}
