#include "processthread.h"

#include <QFile>
#include <QApplication>
#include <QNetworkRequest>
#include <QMetaType>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QTextStream>

ProcessThread::ProcessThread(QObject *parent):
    QThread(parent),
    m_quit(false),
    m_threadDataFinish(false)
{
    qRegisterMetaType <QMap <QString, QString> >("QMap <QString, QString>");
     m_dataFilter = 1.0;

}

ProcessThread::~ProcessThread()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();

    if(m_netManger != NULL)
        m_netManger->deleteLater();

    //wait for the thread has finished.
    wait();
}

void ProcessThread::initialCamera(const QString &addr, QNetworkAccessManager *manager)
{
    m_mutex.lock();
    m_address = addr;
    m_netManger = manager;
    m_mutex.unlock();
}

int ProcessThread::res_position(QString &x, QString &y,
                                QString &z, bool wait)
{
    int res;

    if(!this->isRunning())
        this->start();

    if(wait)
        res = 0;
//    if(wait)
//        res = m_eventLoop.exec(QEventLoop::ExcludeUserInputEvents|
//                                          QEventLoop::ExcludeSocketNotifiers);
    while(!m_threadDataFinish){
        QApplication::processEvents();
    }
    while(m_threadDataFinish){
        if (m_result.contains("Position_X"))
              x = m_result.value("Position_X");

        if (m_result.contains("Position_Y"))
              y = m_result.value("Position_Y");

        if (m_result.contains("Position_Z")){
             z = m_result.value("Position_Z");
        }

        m_threadDataFinish = false;
        QApplication::processEvents();
    }

//    qDebug() << x;
//    qDebug() << objectShape;
    return res;
}

void ProcessThread::readyPoint(QString &ready_point_x, QString &ready_point_y)
{
    if (m_result.contains("Position_X"))
          ready_point_x = m_result.value("Position_X");

    if (m_result.contains("Position_Y"))
          ready_point_y = m_result.value("Position_Y");

//    qDebug() << ready_point_x;
//    qDebug() << ready_point_y;
}

int ProcessThread::res_singleData(const QString &type, QString &data, bool wait)
{
    Q_UNUSED(type)
    Q_UNUSED(data)
    Q_UNUSED(wait)

    return 0;
}

void ProcessThread::run()
{
    QMap<QString, QString>  result;
    QNetworkAccessManager netManager;
    QNetworkReply *reply;
    QNetworkRequest request;

    QTime   m_time = QTime::currentTime();
    QString mm_time = m_time.toString();
    QString str = "**********begin post***********";
    QFile   file("/root/Cameralog.txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
    {
        qDebug() << "Open failed" << endl;
    }
    QTextStream out(&file);
    
    while(1){
    static int count = 0;
    if(count == 2){
        count = 0;
        break;}
    if(count < 2){
        //camera takes picture
        request.setUrl(QUrl(QString("http://%1/CmdChannel?TRIG").arg(m_address)));
       //enter netManager get...
        str = "begin post";
        out<<"["<<mm_time<<"]"<<":"<<str<<endl;         //endl:换行任务

        reply= netManager.get(request);

        str = "**********finish post***********";
        out<<"["<<mm_time<<"]"<<":"<<str<<endl;
       //exit get....

        reply->ignoreSslErrors();
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

        str = "**********relay_finish***********";
        out<<"["<<mm_time<<"]"<<":"<<str<<endl;
        while (!reply->isFinished()) {
            QApplication::processEvents();
        }        
        //wait camera takes photo finisheds
        for(int delay = 0; delay< 200;delay++){
            for(int indelay = 0; indelay < 100; indelay++){
                 QApplication::processEvents();
            }
        }
        //camera analyzes the privious image information
        request.setUrl(QUrl(QString("http://%1/CmdChannel?gRES").arg(m_address)));
        reply= netManager.get(request);

        reply->ignoreSslErrors();
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        str = "**********get_camera_message_finish***********";
        out<<"["<<mm_time<<"]"<<":"<<str<<endl;

        while (!reply->isFinished()) {
            QApplication::processEvents();
        }

        if(reply->error() == QNetworkReply::NoError){
            m_reciveData = reply->readAll();
            emit cameraResult(m_reciveData);
        }
        else{
            //         TODO::
            //         emit errors();
        }
        analyseResult(m_reciveData, result);
        m_result = result;

        reply->deleteLater();
    }
    count++;
  }
    m_threadDataFinish = true;
    str = "**********run_finish***********";
    out<<"["<<mm_time<<"]"<<":"<<str<<endl;
}

void ProcessThread::analyseResult(const QByteArray &array, QMap<QString, QString> &result)
{
    QString string(array);
    QMap<QString, QString> analyseSICKCcharacter;

    if(m_dataFilter){
        QString sentinel = "Position(X,Y,Z)";//Position(X,Y,Z)Center_position:(X,Y,Z)
        QString separation = ",";

        int position = 0;
        int x_relative_position = 0;
        int z_relative_position = 0;

        QString rotate_angle_sentinel = "Rotation:";
        int rotateAnglePosition = 0;

        position =  string.indexOf(sentinel);
        QString tmp_string = string.mid(position+18,30);
        x_relative_position = tmp_string.indexOf(separation);

        QString xPosition = tmp_string.mid(0,x_relative_position);
        QString yPosition = tmp_string.section(',',1,1);
        QString zPositionString = tmp_string.section(',',2,2);
        z_relative_position = zPositionString.indexOf(")");
        QString zPosition = zPositionString.mid(1,z_relative_position-1);

        emit cameraGetPositionX(xPosition);
        emit cameraGetPositionY(yPosition);

        position = string.indexOf(rotate_angle_sentinel);
        QString tmp_rotate_angle = string.mid(position+10,10);
        rotateAnglePosition = tmp_rotate_angle.indexOf("\n");
        tmp_rotate_angle = tmp_rotate_angle.mid(0,rotateAnglePosition);
        m_jointRadian = tmp_rotate_angle;

        m_mutex.lock();
        result["Position_X"] = xPosition;
        result["Position_Y"] = yPosition;
        result["Position_Z"] = zPosition;
        objectShape = "1";
        analyseSICKCcharacter["PositionX"] = xPosition;
        analyseSICKCcharacter["PositionY"] = yPosition;
        analyseSICKCcharacter["PositionZ"] = zPosition;
        m_mutex.unlock();
        emit cameraCharacter(analyseSICKCcharacter);

        m_selectObjectFlag = xPosition;

        static double dataMean = 1.0;

        dataMean = m_selectObjectFlag.toDouble();
        if(0.0 == dataMean){
            m_mutex.lock();
            result["Position_X"] = "-400.3";
            result["Position_Y"] = "-121.5";
            result["Position_Z"] = "379.66";
            objectShape = "0";
            analyseSICKCcharacter["PositionX"] = xPosition;
            analyseSICKCcharacter["PositionY"] = yPosition;
            analyseSICKCcharacter["PositionZ"] = zPosition;
            m_mutex.unlock();
        }




//        emit cameraCharacter(analyseSICKCcharacter);
    }

//     m_recSickCharacter = analyseSICKCcharacter;

}

void ProcessThread::recSickCharacter(QMap<QString, QString> &recSICKCharacter)
{
    recSICKCharacter = m_recSickCharacter;
}































