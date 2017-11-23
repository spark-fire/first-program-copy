#include "sick_pimxpluginwidget.h"
#include "ui_sick_pimxpluginwidget.h"

#include "cameracontrol.h"
#include "bearingplace.h"
#include <QTimer>
#include <QWebEngineView>
#include <QFileDialog>
#include <QDialog>
#include <QList>
#include <QDebug>
#include <QHBoxLayout>

QString data_get;

SICK_PIMxPluginWidget::SICK_PIMxPluginWidget(CameraControl *camera, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SICK_PIMxPluginWidget),
    m_camera(camera)
{
    ui->setupUi(this);
    initWgt();

    connect(parent,SIGNAL(signal_sendRealTimeRoadPoint(QString)),this,SLOT(slot_recvRealTimeRoadPoint(QString)));


    m_view = new QWebEngineView(/*ui->camerViewFrame*/this);
    m_view->setFixedSize(640,480);
    QHBoxLayout *m_cameraLayout = new QHBoxLayout(this);
    m_cameraLayout->addWidget(m_view);
    ui->groupBox_5->setLayout(m_cameraLayout);
    ui->groupBox_5->layout()->setContentsMargins(5,5,5,5);
    //camera IP show
    ui->le_cameraIPAddress->setText("192.168.0.100");


    m_inputMessage = new QTableWidget(0,2,ui->widget_tablePosition);
    m_inputMessage->verticalHeader()->setVisible(false);
    //the whole line is selected
    m_inputMessage->setSelectionBehavior(QAbstractItemView::SelectRows);
    //add Lyj 2017-11-1
    QHBoxLayout *m_tablePosWgtLayout = new QHBoxLayout(this);
    m_tablePosWgtLayout->addWidget(m_inputMessage);
    ui->widget_tablePosition->setLayout(m_tablePosWgtLayout);
    ui->widget_tablePosition->layout()->setContentsMargins(1,1,1,1);
    m_inputMessage->setColumnWidth(0,400);
    m_inputMessage->setColumnWidth(1,400);
    //---end---

    QStringList headLable;
    headLable << "message" << "value";
    m_inputMessage->setHorizontalHeaderLabels(headLable);

    //display all kinds pictures from camera
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SICK_PIMxPluginWidget::slotTimeout);
    //camera sanpshot
    m_netManger = new QNetworkAccessManager;
    connect(m_netManger, SIGNAL(finished(QNetworkReply*)),
                         this, SLOT(slotReplyPixmapLoad(QNetworkReply*)));
    //ui display camera result
    connect(m_camera, SIGNAL(camera_data(QString)) ,
                   ui->tbr_cameraData, SLOT(setText(QString)));
    connect(m_camera,SIGNAL(camera_position_x(QString)),
                            ui->le_xPosition,SLOT(setText(QString)));
    connect(m_camera,SIGNAL(camera_position_y(QString)),
                            ui->le_yPosition,SLOT(setText(QString)));
    //refresh camera character data
//    connect(m_camera,SIGNAL(camera_character(QMap<QString,QString>)),
//                            this,SLOT(soltRefreshCameraCharacter()));
    connect(m_camera,&CameraControl::camera_character,
                            this,&SICK_PIMxPluginWidget::soltRefreshCameraCharacter);
}

SICK_PIMxPluginWidget::~SICK_PIMxPluginWidget()
{
    delete ui;
}

void SICK_PIMxPluginWidget::slotTimeout()
{
    m_view->load(m_imageUrl);
    m_view->show();
}

void SICK_PIMxPluginWidget::slotReplyPixmapLoad(QNetworkReply *reply)
{
    QPixmap pixmap;
    QString format = "png";
    QDateTime dateTime = QDateTime::currentDateTime();
    QString fileName =  m_saveDirectory + "/" + dateTime.toString("yyyyMMddhhmmsszzz")+"."+ format;

    if (reply->error() == QNetworkReply::NoError){
        pixmap.loadFromData(reply->readAll());
        pixmap.save(fileName);
    }else{
        qDebug()<< reply->errorString();
    }

//    m_netManger->deleteLater();
}

void SICK_PIMxPluginWidget::on_pbn_connect_clicked()
{
    ui->le_cameraIPAddress->setText("192.168.0.100");
    m_camera->setCameraAddress("192.168.0.100");

    QString x,y,z;
    double test;
//    m_camera->setCameraAddress(ui->le_cameraIPAddress->text());
//    QNetworkReply *reply;
//    QNetworkAccessManager netManger;
//    //camera switchs object
//    reply = netManger.get(QNetworkRequest(QUrl(QString("http://%1/CmdChannel?sINT_1_1")
//                                               .arg(m_camera->cameraAddress()))));

//    reply->ignoreSslErrors();
//    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

//    while (!reply->isFinished()) {
//        QApplication::processEvents();
//    }
    m_camera->cam_position(x, y,z);

    m_camera->cam_joint_radian(test);
    qDebug() << test;
}

void SICK_PIMxPluginWidget::on_pbn_showHideImage_clicked()
{
    m_imageUrl = QString("http://%1/LiveImage.jpg")
            .arg(m_camera->cameraAddress());
    //start refresh
    slotTimeout();
    m_timer->start(1200);
}

void SICK_PIMxPluginWidget::on_rbn_Overlay_clicked()
{
    m_imageUrl = QString("http://%1/LiveImage.jpg?ShowOverlay")
            .arg(m_camera->cameraAddress());
}

void SICK_PIMxPluginWidget::on_rbn_RawData_clicked()
{
    m_imageUrl = QString("http://%1/LiveImage.jpg")
            .arg(m_camera->cameraAddress());
}

void SICK_PIMxPluginWidget::on_rbn_SimpleOverlay_clicked()
{
    m_imageUrl = QString("http://%1/LiveImage.jpg?SimplifiedOverlay")
            .arg(m_camera->cameraAddress());
}

void SICK_PIMxPluginWidget::on_pbn_snapShot_clicked()
{
    m_netManger->get(QNetworkRequest(m_imageUrl));
}

void SICK_PIMxPluginWidget::on_tbn_imageSave_clicked()
{
    m_saveDirectory = QFileDialog::getExistingDirectory
            (this, tr("Open Directory"),
             "/home",
             QFileDialog::ShowDirsOnly
             | QFileDialog::DontResolveSymlinks);
    ui->le_imageSaveDisplay->setText(m_saveDirectory);
}

//void SICK_PIMxPluginWidget::on_pbn_toolOffsetSave_clicked()
//{
//    m_camera->setToolOffset(ui->le_xoffset->text(),
//                            ui->le_yoffset->text(),
//                            ui->le_zoffset->text());
//}

void SICK_PIMxPluginWidget::on_pbn_compensation_clicked()
{
    m_camera->setCoordinateCompensation(ui->le_xcompensation->text(),
                                        ui->le_ycompensation->text(),
                                        ui->le_zcompensation->text());
}

void SICK_PIMxPluginWidget::on_pbn_searchSickCharacter_clicked()
{
    m_searchName << ui->le_searchSickCharacter->text();
    int returnnum = searchCharacter();

    if(returnnum){
    ;}else{
        QDialog *dialog = new QDialog(ui->tab);
        dialog->setWindowTitle((tr("failed")));
        dialog->show();
    }

//    m_searchName.removeLast();

}

int SICK_PIMxPluginWidget::searchCharacter()
{
    QMap<QString, QString> SICKCharacter;
    m_camera->getSICKCharacter(SICKCharacter);
    QMap<QString, QString>::iterator it;

    for(it=SICKCharacter.begin(); it!=SICKCharacter.end();++it)
    {
        int i = 0;
        m_inputMessage->insertRow(i);
        m_inputMessage->setItem(i,0,new QTableWidgetItem(it.key()));
        m_inputMessage->setItem(i,1,new QTableWidgetItem(it.value()));
        if(i != SICKCharacter.count()){
            i++;
        }
    }
    return 0;
}

void SICK_PIMxPluginWidget::on_pbn_delete_clicked()
{
    int rowIndex = m_inputMessage->currentRow();

    if(rowIndex != -1){
        m_inputMessage->removeRow(rowIndex);
    }
    m_deleteCharacterPosition << rowIndex;
}

void SICK_PIMxPluginWidget::soltRefreshCameraCharacter(QMap <QString, QString> character)
{
    m_inputMessage->setRowCount(0);
    m_inputMessage->clearContents();
    QMap<QString, QString>::iterator it;
    int i = 0;
    int flag = 1;

    for(it=character.begin(); it!=character.end();++it)
    {
        int deleteCount = m_deleteCharacterPosition.size();

        if(0 == deleteCount){
            m_inputMessage->insertRow(i);
            m_inputMessage->setItem(i,0,new QTableWidgetItem(it.key()));
            m_inputMessage->setItem(i,1,new QTableWidgetItem(it.value()));
            if(i != character.count()){
                i++;
            }
        }else{
                for(int j = 0; j < m_deleteCharacterPosition.size();j++){
                    if(i == m_deleteCharacterPosition.at(j)){
                        flag = 0;
                    }
                }

                if(flag){
                    m_inputMessage->insertRow(i);
                    m_inputMessage->setItem(i,0,new QTableWidgetItem(it.key()));
                    m_inputMessage->setItem(i,1,new QTableWidgetItem(it.value()));
                }
                if(i != character.count()){
                    i++;
                }
            }
    }
}

void SICK_PIMxPluginWidget::initWgt()
{
    m_pluginView = new QWebView(this);
    QHBoxLayout *pluginWebLayout = new QHBoxLayout(this);
    pluginWebLayout->addWidget(m_pluginView);
    ui->widget_pluginWeb->setLayout(pluginWebLayout);
    QString m_url = "qrc:/InstructionWeb/aubocCamera.html";
    m_pluginView->load(m_url);
}

void SICK_PIMxPluginWidget::setPluginInfomation(QString organization,
                                                QString whatsThis,
                                                QString name,
                                                QString author,
                                                QString version)
{
    ui->label_pluginInstruction->setText(QString("%1\n%2\n%3\n%4\n%5\n")
                                         .arg(organization)
                                         .arg(whatsThis)
                                         .arg(name)
                                         .arg(author)
                                         .arg(version));
}

void SICK_PIMxPluginWidget::on_pbn_initCamera_clicked()
{
    QNetworkReply *reply;
    QNetworkAccessManager netManger;
    m_camera->initCameraObject();

    reply = netManger.get(QNetworkRequest(QUrl(QString("http://%1/CmdChannel?sINT_1_1")
                                               .arg(m_camera->cameraAddress()))));
    reply->ignoreSslErrors();
    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    while (!reply->isFinished()) {
        QApplication::processEvents();
    }
    reply= netManger.get(QNetworkRequest(QUrl(QString("http://%1/CmdChannel?TRIG")
                                               .arg(m_camera->cameraAddress()))));
    reply->ignoreSslErrors();
    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    while (!reply->isFinished()) {
        QApplication::processEvents();
    }
}

//void SICK_PIMxPluginWidget::on_pbn_test_clicked()
//{
//    QString pos_x, pos_y, pos_z;
//    QString x,y;
//    QString compensationx,compensationy,compensationz;
//    double numpos_x = 0.0;
//    double numpos_y = 0.0;
//    double numpos_z = 0.0;
//    m_camera->cam_position(pos_x, pos_y, pos_z);

//    m_camera->getCoordinateCompensation(compensationx,compensationy,
//                                        compensationz);
//    m_camera->cam_ready_point(x,y);
//    numpos_x = (pos_x.toDouble()) / 1000.0 + compensationx.toDouble()/1000;
//    pos_x = QString::number(numpos_x);
//    numpos_y = (pos_y.toDouble()) / 1000.0 + compensationy.toDouble()/1000;
//    pos_y = QString::number(numpos_y);
//    numpos_z = (pos_z.toDouble()) / 1000.0;
//    pos_z = QString::number(numpos_z);

////    ui->le_xPosition->setText(pos_x);
////    ui->le_yPosition->setText(pos_y);
////    ui->le_radious->setText(pos_z);
//}

void SICK_PIMxPluginWidget::slot_recvRealTimeRoadPoint(QString Data)
{
    data_get = Data;
    //qDebug() << data_get;
//    ui->textBrowser->setText(data_get);

    if(data_get.mid(80,2) == "-0"){
        ui->le_begin_value_x1->setText(data_get.mid(80,9));

        if(data_get.mid(10,2) == "-0"){
            ui->le_begin_value_y1->setText(data_get.mid(10,9));

            if(data_get.mid(20,2) == "-0"){
                ui->le_begin_value_z1->setText(data_get.mid(20,9));
            }

            if(data_get.mid(20,2) == "0."){
                ui->le_begin_value_z1->setText(data_get.mid(20,8));
            }

        }


        if(data_get.mid(10,2) == "0."){
            ui->le_begin_value_y1->setText(data_get.mid(10,8));

            if(data_get.mid(19,2) == "-0"){
                ui->le_begin_value_z1->setText(data_get.mid(19,9));
            }

            if(data_get.mid(19,2) == "0."){
                ui->le_begin_value_z1->setText(data_get.mid(19,8));
            }
        }

    }

}


//void SICK_PIMxPluginWidget::on_pbn_Save_clicked()
//{
//    double beginPos_x = 0.5586;
//    double beginPos_y = 0.0261;
//    double beginPos_z1 = 0.5718;
//    double beginPos_z2 = 0.3750;

//    initScriptProgect();

//    int bearingNum = 4;
//    int arrayCount = 0;
//    BearingPlace pointPosition[bearingNum];

//    for(int j=0; j<bearingNum/2; j++){
//        for(int i=0; i<bearingNum/2; i++){
//            pointPosition[arrayCount].SetWaypoint_x(beginPos_x - 0.07*i);
//            pointPosition[arrayCount].SetWaypoint_y(beginPos_y + 0.07*j);
//            pointPosition[arrayCount].SetWaypoint_z(beginPos_z2);
//            arrayCount++;
//        }
//    }

//    camera_1ScriptFile(pointPosition[0].GetWaypoint_x(),pointPosition[0].GetWaypoint_y(),
//                     beginPos_z1,pointPosition[0].GetWaypoint_z());

//}

void SICK_PIMxPluginWidget::initScriptProgect()
{
    QString ScriptFileName = "/root/AuboRobotWorkSpace/OUR-i5/bin/Script/Camera1.aubo";
    QFile file(ScriptFileName);
    file.resize(0);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        qDebug() << "can't open the file!";
    }

    else
    {
        QString str_init1 = "init_global_move_profile()";
        QString str_init2 = "set_joint_maxacc({1.308997,1.308997,1.308997,1.570796,1.570796,1.570796})";
        QString str_init3 = "set_joint_maxvelc({1.308997,1.308997,1.308997,1.570796,1.570796,1.570796})";
        QTextStream in(&file);
        in << str_init1 << "\n";
        in << str_init2 << "\n";
        in << str_init3 << "\n";

    }
}

void SICK_PIMxPluginWidget::camera_1ScriptFile()
{
    QString ScriptFileName = "/root/AuboRobotWorkSpace/OUR-i5/bin/Script/Camera1.aubo";
    QFile file(ScriptFileName);


    if(!file.open(QIODevice::ReadWrite | QIODevice::Append)){
        qDebug() << "can't open the file!";
    }else{
        //qDebug() << "open file success";
        QString str11 = "move_joint({0.0,-0.127264,-1.32111,0.376933,-1.570796,0.0},true)";
        // Teaching position of takephoto
        //QString str12 = tr("move_joint_to_target_position_keep_current_ori({%1,%2,%3},CoordType.BaseCoordinate,{0.0,0.0,0.0},true)").arg(ui->le_begin_value_x1->text())
        //                                                                                                                              .arg(ui->le_begin_value_y1->text())
        //                                                                                                                             .arg(ui->le_begin_value_z1->text());
        QString str12 = "move_joint({0.0,-0.127264,-1.32111,0.376933,-1.570796,0.0},true)";
        QString str1open0 = "set_modbus_io_status(\"M_CO2\", 0)";
        QString str1_open = "set_modbus_io_status(\"M_CO1\", 1)";
        QString str13 = "move_joint(get_waypoint_from_camera(0.0124,0.0180,0.399,true),true)";
        QString str14 = "move_line(get_waypoint_from_camera(0.0124,0.01801,0.32199,false),true)";
        QString str1_close0 = "set_modbus_io_status(\"M_CO1\", 0)";
        QString str1_close = "set_modbus_io_status(\"M_CO2\", 1)";
        QString str15 = "move_line(get_waypoint_from_camera(0.0124,0.01801,0.32199,false),true)";
        //robot come back, not sure require
        QString str16 = "move_joint_to_target_position_keep_current_ori({-0.40032,-0.12150,0.71542},CoordType.BaseCoordinate,{0.0,0.0,0.0},true)";
        //robot down
        QString str161 = "move_joint({0.0,-0.127264,-1.32111,0.376933,-1.570796,0.0},true)";
        QString str17 = "move_joint_to_target_position_keep_current_ori({-0.40032,-0.12150,0.21542},CoordType.BaseCoordinate,{0.0,0.0,0.0},true)";
        QString str18 = "move_line_to_target_position_keep_current_ori({-0.40032,-0.12150,0.71542},CoordType.BaseCoordinate,{0.0,0.0,0.0},true)";
        QString str19 = "move_line_to_target_position_keep_current_ori({-0.40032,-0.12150,0.71542},CoordType.BaseCoordinate,{0.0,0.0,0.0},true)";

        QTextStream in(&file);

        in << str11 << "\n";
        in << str12 << "\n";
        in << str1open0 << "\n";
        in << str1_open << "\n";
        in << str13 << "\n";
        in << str14 << "\n";
        in << str1_close0 << "\n";
        in << str1_close << "\n";
        in << "sleep(2)" << "\n";
        in << str15 << "\n";
        in << str16 << "\n";
        in << str161 << "\n";
        in << str11 << "\n";  //init position
        in << str17 << "\n";
        in << str18 << "\n";
        in << str1open0 << "\n";
        in << str1_open << "\n";
        in << str19 << "\n";
        in << str11 << "\n";
        in << "\n";

    }
}

void SICK_PIMxPluginWidget::camera_2ScriptFile()
{
    QString ScriptFileName = "/root/AuboRobotWorkSpace/OUR-i5/bin/Script/Camera2.aubo";
    QFile file(ScriptFileName);
    file.resize(0);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        qDebug() << "can't open the file!";
    }

    else
    {
        QString str_init1 = "init_global_move_profile()";
        QString str_init2 = "set_joint_maxacc({1.308997,1.308997,1.308997,1.570796,1.570796,1.570796})";
        QString str_init3 = "set_joint_maxvelc({1.308997,1.308997,1.308997,1.570796,1.570796,1.570796})";

        QString str21 = "move_joint({0.0,-0.127264,-1.32111,0.376933,-1.570796,0.0},true)";
        QString str22 = tr("local camera_agv_positionA = {%1,%2,%3}").arg(ui->le_begin_value_x2->text())
                                                                     .arg(ui->le_begin_value_y2->text())
                                                                     .arg(ui->le_begin_value_z2->text());
        QString str23 = tr("local camera_agv_positionB = {%1,%2,%3}").arg(ui->le_begin_value_x3->text())
                                                                     .arg(ui->le_begin_value_y3->text())
                                                                     .arg(ui->le_begin_value_z3->text());
        QString str24 = "local camera_agv_arr = {}";
        QString str25 = "camera_agv_arr[0] = camera_agv_positionA";
        QString str26 = "camera_agv_arr[1] = camera_agv_positionB";
        QString str27 = "for camera_agv_count = 0,2,1";
        QString str28 = "do";
        //consern times of paw
        QString str20_open0 = "set_modbus_io_status(\"M_CO2\", 0)";
        QString str20_open = "set_modbus_io_status(\"M_CO1\", 1)";

        // Teaching position1 of takephoto
        QString str29 = "move_joint_to_target_position_keep_current_ori(camera_agv_arr[camera_agv_count],CoordType.BaseCoordinate,{0.0,0.0,0.0},true)";
        QString str30 = "move_joint(get_waypoint_from_camera(-0.1377-camera_agv_count*0.02,0.0110+camera_agv_count*0.07,0.332,true),true)";
        QString str30_1 = "if (1 == sorting_shape()) then";
        QString str31 = "move_line(get_waypoint_from_camera(-0.1377-camera_agv_count*0.02,0.0110+camera_agv_count*0.07,0.2322,false),true)";
        QString str20_close0 = "set_modbus_io_status(\"M_CO1\", 0)";
        QString str20_close = "set_modbus_io_status(\"M_CO2\", 1)";
        QString str32 = "move_line(get_waypoint_from_camera(-0.1377-camera_agv_count*0.02,0.0110+camera_agv_count*0.07,0.371,false),true)";
        QString str33 = "move_joint({0.0,-0.127264,-1.32111,0.376933,-1.570796,0.0},true)";
        QString str34 = "break";
        QString str35 = "else ";
        QString str351 = "end";
        QString str36 = "end";


        QTextStream in(&file);
        in << str_init1 << "\n";
        in << str_init2 << "\n";
        in << str_init3 << "\n";
        in << str21 << "\n";
        in << str22 << "\n";
        in << str23 << "\n";
        in << str24 << "\n";
        in << str25 << "\n";
        in << str26 << "\n";
        in << str27 << "\n";
        in << str28 << "\n";
        in << str20_open0 << "\n";
        in << str20_open << "\n";
        in << str29 << "\n";
        in << str30 << "\n";
        in << str30_1 << "\n";
        in << str31 << "\n";
        in << str20_close0 << "\n";
        in << str20_close << "\n";
        in << "sleep(2)" << "\n";
        in << str32 << "\n";
        in << str33 << "\n";
        in << str34 << "\n";
        in << str35 << "\n";
        in << str351 << "\n";
        in << str36 << "\n";

    }
}


void SICK_PIMxPluginWidget::on_pbn_teach_check1_clicked()
{
    if(data_get.mid(0,2) == "-0"){
        ui->le_begin_value_x1->setText(data_get.mid(0,9));

        if(data_get.mid(10,2) == "-0"){
            ui->le_begin_value_y1->setText(data_get.mid(10,9));

            if(data_get.mid(20,2) == "-0"){
                ui->le_begin_value_z1->setText(data_get.mid(20,9));
            }

            if(data_get.mid(20,2) == "0."){
                ui->le_begin_value_z1->setText(data_get.mid(20,8));
            }

        }


        if(data_get.mid(10,2) == "0."){
            ui->le_begin_value_y1->setText(data_get.mid(10,8));

            if(data_get.mid(19,2) == "-0"){
                ui->le_begin_value_z1->setText(data_get.mid(19,9));
            }

            if(data_get.mid(19,2) == "0."){
                ui->le_begin_value_z1->setText(data_get.mid(19,8));
            }
        }

    }


    if(data_get.mid(0,2) == "0."){
        ui->le_begin_value_x1->setText(data_get.mid(0,8));

        if(data_get.mid(9,2) == "-0"){
            ui->le_begin_value_y1->setText(data_get.mid(9,9));

            if(data_get.mid(19,2) == "-0"){
                ui->le_begin_value_z1->setText(data_get.mid(19,9));
            }

            if(data_get.mid(19,2) == "0."){
                ui->le_begin_value_z1->setText(data_get.mid(19,8));
            }
        }

        if(data_get.mid(9,2) == "0."){
            ui->le_begin_value_y1->setText(data_get.mid(9,8));

            if(data_get.mid(18,2) == "-0"){
                ui->le_begin_value_z1->setText(data_get.mid(18,9));
            }
            if(data_get.mid(18,2) == "0."){
                ui->le_begin_value_z1->setText(data_get.mid(18,8));
            }
        }

    }

    initScriptProgect();
    camera_1ScriptFile();
}

void SICK_PIMxPluginWidget::on_pbn_teach_check2_clicked()
{
    if(data_get.mid(0,2) == "-0"){
        ui->le_begin_value_x2->setText(data_get.mid(0,9));

        if(data_get.mid(10,2) == "-0"){
            ui->le_begin_value_y2->setText(data_get.mid(10,9));

            if(data_get.mid(20,2) == "-0"){
                ui->le_begin_value_z2->setText(data_get.mid(20,9));
            }

            if(data_get.mid(20,2) == "0."){
                ui->le_begin_value_z2->setText(data_get.mid(20,8));
            }

        }


        if(data_get.mid(10,2) == "0.")
        {
            ui->le_begin_value_y2->setText(data_get.mid(10,8));

            if(data_get.mid(19,2) == "-0"){
                ui->le_begin_value_z2->setText(data_get.mid(19,9));
            }

            if(data_get.mid(19,2) == "0."){
                ui->le_begin_value_z2->setText(data_get.mid(19,8));
            }

        }

    }


    if(data_get.mid(0,2) == "0.")
    {
        ui->le_begin_value_x2->setText(data_get.mid(0,8));

        if(data_get.mid(9,2) == "-0"){
            ui->le_begin_value_y2->setText(data_get.mid(9,9));

            if(data_get.mid(19,2) == "-0"){
                ui->le_begin_value_z2->setText(data_get.mid(19,9));
            }

            if(data_get.mid(19,2) == "0."){
                ui->le_begin_value_z2->setText(data_get.mid(19,8));
            }
        }

        if(data_get.mid(9,2) == "0.")
        {
            ui->le_begin_value_y2->setText(data_get.mid(9,8));

            if(data_get.mid(18,2) == "-0"){
                ui->le_begin_value_z2->setText(data_get.mid(18,9));
            }
            if(data_get.mid(18,2) == "0."){
                ui->le_begin_value_z2->setText(data_get.mid(18,8));
            }
        }

    }

    camera_2ScriptFile();
}

void SICK_PIMxPluginWidget::on_pbn_teach_check3_clicked()
{
    if(data_get.mid(0,2) == "-0")
    {
        ui->le_begin_value_x3->setText(data_get.mid(0,9));

        if(data_get.mid(10,2) == "-0"){
            ui->le_begin_value_y3->setText(data_get.mid(10,9));

            if(data_get.mid(20,2) == "-0"){
                ui->le_begin_value_z3->setText(data_get.mid(20,9));
            }

            if(data_get.mid(20,2) == "0."){
                ui->le_begin_value_z3->setText(data_get.mid(20,8));
            }

        }


        if(data_get.mid(10,2) == "0.")
        {
            ui->le_begin_value_y3->setText(data_get.mid(10,8));

            if(data_get.mid(19,2) == "-0"){
                ui->le_begin_value_z3->setText(data_get.mid(19,9));
            }

            if(data_get.mid(19,2) == "0."){
                ui->le_begin_value_z3->setText(data_get.mid(19,8));
            }

        }

    }


    if(data_get.mid(0,2) == "0.")
    {
        ui->le_begin_value_x3->setText(data_get.mid(0,8));

        if(data_get.mid(9,2) == "-0"){
            ui->le_begin_value_y3->setText(data_get.mid(9,9));

            if(data_get.mid(19,2) == "-0"){
                ui->le_begin_value_z3->setText(data_get.mid(19,9));
            }

            if(data_get.mid(19,2) == "0."){
                ui->le_begin_value_z3->setText(data_get.mid(19,8));
            }
        }

        if(data_get.mid(9,2) == "0."){
            ui->le_begin_value_y3->setText(data_get.mid(9,8));

            if(data_get.mid(18,2) == "-0"){
                ui->le_begin_value_z3->setText(data_get.mid(18,9));
            }
            if(data_get.mid(18,2) == "0."){
                ui->le_begin_value_z3->setText(data_get.mid(18,8));
            }
        }

    }

    camera_2ScriptFile();
}

//void SICK_PIMxPluginWidget::on_pbn_teach_check4_clicked()
//{
//    if(data_get.mid(0,2) == "-0")
//    {
//        ui->le_begin_value_x4->setText(data_get.mid(0,9));

//        if(data_get.mid(10,2) == "-0"){
//            ui->le_begin_value_y4->setText(data_get.mid(10,9));

//            if(data_get.mid(20,2) == "-0"){
//                ui->le_begin_value_z4->setText(data_get.mid(20,9));
//            }

//            if(data_get.mid(20,2) == "0."){
//                ui->le_begin_value_z4->setText(data_get.mid(20,8));
//            }

//        }


//        if(data_get.mid(10,2) == "0.")
//        {
//            ui->le_begin_value_y4->setText(data_get.mid(10,8));

//            if(data_get.mid(19,2) == "-0"){
//                ui->le_begin_value_z4->setText(data_get.mid(19,9));
//            }

//            if(data_get.mid(19,2) == "0."){
//                ui->le_begin_value_z4->setText(data_get.mid(19,8));
//            }

//        }

//    }


//    if(data_get.mid(0,2) == "0.")
//    {
//        ui->le_begin_value_x4->setText(data_get.mid(0,8));

//        if(data_get.mid(9,2) == "-0"){
//            ui->le_begin_value_y4->setText(data_get.mid(9,9));

//            if(data_get.mid(19,2) == "-0"){
//                ui->le_begin_value_z4->setText(data_get.mid(19,9));
//            }

//            if(data_get.mid(19,2) == "0."){
//                ui->le_begin_value_z4->setText(data_get.mid(19,8));
//            }
//        }

//        if(data_get.mid(9,2) == "0."){
//            ui->le_begin_value_y4->setText(data_get.mid(9,8));

//            if(data_get.mid(18,2) == "-0"){
//                ui->le_begin_value_z4->setText(data_get.mid(18,9));
//            }
//            if(data_get.mid(18,2) == "0."){
//                ui->le_begin_value_z4->setText(data_get.mid(18,8));
//            }
//        }

//    }
//}
