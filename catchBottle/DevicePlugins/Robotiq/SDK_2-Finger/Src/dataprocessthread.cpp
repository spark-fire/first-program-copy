#include "dataprocessthread.h"
#include "checksum.h"

#include <QDataStream>
#include <QDebug>

DataProcessThread::DataProcessThread(QObject *parent)
    : QThread(parent),
     m_isCurrentCmdResponse(false),
     m_waitMode(true),
     m_quit(false)
{
    m_theLastReadRequest.clear();
    m_theLastWriteRequest.clear();
    m_currentRequest.clear();
    m_theLastReadRequest.resize(8);
    m_theLastWriteRequest.resize(15);

}

DataProcessThread::~DataProcessThread()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();
    //wait for the thread has finished.
    wait();
}

void DataProcessThread::startDataThread(GripperSettings &settings, GripperStatus &status)
{
    m_mutex.lock();
    m_settings = settings;
    m_status = status;
    m_mutex.unlock();

    if(!isRunning())
        this->start();
}

int DataProcessThread::rq_clear()
{
    //return this->rq_actions(GripperCommand::GRIPPER_CLEAR, false);
}

int  DataProcessThread::rq_active(bool wait)
{
    //return this->rq_actions(GripperCommand::GRIPPER_ACTIIVE, wait);
}

int DataProcessThread::rq_open(GripperSettings settings, bool wait)
{
    m_settings = settings;
    //return this->rq_actions(GripperCommand::GRIPPER_OPEN, wait);
}

int  DataProcessThread::rq_close(GripperSettings settings, bool wait)
{
    m_settings = settings;
    //return this->rq_actions(GripperCommand::GRIPPER_CLOSE, wait);
}

int  DataProcessThread::rq_move(GripperSettings settings, bool wait)
{
    m_settings = settings;
    //return this->rq_actions(GripperCommand::GRIPPER_MOVE, wait);
}

void DataProcessThread::run()
{
    QPair<int, bool> tmp;
    while(!m_quit) {


        //check request queue
        while(!m_queue.isEmpty()){

            int timeoutCount = 100;
            bool error = false;

            // STEP 1: construct resquest command
            if(m_queue.count() == 0)
                return;
            m_mutex.lock();
            tmp = m_queue.dequeue();
            m_mutex.unlock();

            processWriteRequest(static_cast<GripperCommand>(tmp.first));
            emit this->sendRequest(m_currentRequest);
            qDebug() <<" Thread request :" << m_currentRequest.toHex();

            // STEP 2: test whehter get correct response
            while(!m_isCurrentCmdResponse)
            {
                if(--timeoutCount > 0){
                    usleep(600);
                }else{
                    error = true;
                    emit this->error(QString("Write request command send timeout...."));
                    break;
                }
            }

            //STEP3: test the action is complete.
            if(!error){
                emit this->status(QString("Command recive correct response"));

                //STEP4: in wait mode
                if(tmp.second){
                    m_mutex.lock();
                    int retryCount = 50;
                    timeoutCount = 100;
                    m_isCurrentCmdResponse = false;
                    m_mutex.unlock();

                    //STEP 5: construct read request
                    processReadRequest();

                    //STEP6: retry 100 times for check action is completed.
                    while(retryCount--){
                        emit this->sendRequest(m_currentRequest);
//                        qDebug() <<"No." << retryCount << " : Thread request :" << m_currentRequest.toHex();

                        // STEP 7: test whehter get correct response
                        while(!m_isCurrentCmdResponse)
                        {
                            if(--timeoutCount > 0){
                                usleep(500);
                            }else{
                                error = true;
                                emit this->error(QString("Read request command send timeout...."));
                                break;
                            }
                        }
                        // STEP 8: test whehter get complete action
                        if(!error){
                            // STEP8: gripper is complete action
                            if(m_status.status() == 0x31 || m_status.status() == 0xF9){
                                if(m_eventloop.isRunning()){
                                    m_eventloop.quit();
                                }
                                emit this->status(QString("Gripper has complete action."));
                                break;
                            }
                        }

                        // sleep for next retry
                        msleep(100);
                    }
                }

            }

        }
        if(m_eventloop.isRunning()){
            m_eventloop.exit(1982);
        }
    }
}

void DataProcessThread::slotReciveRespone(const QByteArray &data)
{
    if(data.length() < 8)
        return ;
    //STEP1 : check crc
    quint8 crcHi = static_cast<quint8>(data.at(data.length()-2));
    quint8 crclo = static_cast<quint8>(data.at(data.length()-1));
    quint16 crc16 = Checksum::crc16ForModbus(data.left(data.length()-2));

//    qDebug() << "Response CRC: " << QString::number(crcHi, 16) << " "
//             << QString::number(crclo, 16) << endl
//             << "Cacl CRC: " << QString::number(crc16, 16);

    if(crcHi != quint8(crc16) || crclo != quint8(crc16 >> 8))
        return;

    // STEP2 : the resopnse is for write request ?
    if(Gripper::g_cmdPresetMultiReg == data.at(1)){

        if(data.left(data.size()-2) == m_currentRequest.left(data.size()-2)){

            m_mutex.lock();
            m_isCurrentCmdResponse = true;
            m_mutex.unlock();

        }else{
            // if error
            // tell serial to clear its recive buffer
            emit this->error(QString("preset multi registers response is wrong..."));
        }

    // STEP3: the response is for read request ?
    }else if(Gripper::g_cmdReadHoldingReg == data.at(1) ){

        if(data.left(2) == m_currentRequest.left(2)){

            m_mutex.lock();
            m_isCurrentCmdResponse = true;
            // update gripper status
            m_status.setStatus(data.at(3+0));
            m_status.setFault(data.at(3+2));
            m_status.setPosRequest(data.at(3+3));
            m_status.setPosAcutal(data.at(3+4));
            m_status.setCurrent(data.at(3+5));
            m_mutex.unlock();

        }else{
            // if error
            // tell serial to clear its recive buffer
            emit this->error(QString("Read holding register response is wrong..."));
        }
    }
}

void DataProcessThread::writeRequestHeader(QByteArray &req)
{
    QDataStream  out(&req, QIODevice::WriteOnly);

    out << quint8(Gripper::g_ID)
        << quint8(Gripper::g_cmdPresetMultiReg)
        << quint16(Gripper::g_regInput)
        << quint16(Gripper::g_regCount)
        << quint8(Gripper::g_regCount * 2);
}

void DataProcessThread::readRequestHeader(QByteArray &req)
{
    QDataStream  out(&req, QIODevice::WriteOnly);

    out << quint8(Gripper::g_ID)
        << quint8(Gripper::g_cmdReadHoldingReg)
        << quint16(Gripper::g_regOutput)
        << quint16(Gripper::g_regCount);
}

void DataProcessThread::processWriteRequest(GripperCommand cmd)
{
    m_mutex.lock();
    m_isCurrentCmdResponse = false;
    m_currentRequest.clear();
    writeRequestHeader(m_currentRequest);
    QDataStream out(&m_currentRequest, QIODevice::Append);

    switch(cmd){
    case GRIPPER_CLEAR:
        out << quint16(0)
            << quint16(0)
            << quint16(0);
        break;
    case GRIPPER_ACTIIVE:
        out << quint16(0x0100)
            << quint16(0)
            << quint16(0);
        break;
    case GRIPPER_CLOSE:
        out << quint16(0x9 << 8)
            << quint16(0xFF)
            << quint8(m_settings.speed())
            << quint8(m_settings.force());
        break;
    case GRIPPER_OPEN:
        out << quint16(0x9 << 8)
            << quint16(0)
            << quint8(m_settings.speed())
            << quint8(m_settings.force());
        break;
    case GRIPPER_MOVE:
        out << quint16(0x9 << 8)
            << quint16(m_settings.position())
            << quint8(m_settings.speed())
            << quint8(m_settings.force());

    default:
        ;
    }

    quint16 crc16 = Checksum::crc16ForModbus(m_currentRequest);

    out << quint8(crc16)
        << quint8(crc16 >> 8);

    m_currentRequest.resize(15);
    m_mutex.unlock();
}

void DataProcessThread::processReadRequest()
{
    m_mutex.lock();
    m_isCurrentCmdResponse = false;
    m_currentRequest.clear();

    readRequestHeader(m_currentRequest);
    QDataStream out(&m_currentRequest, QIODevice::Append);

    quint16 crc16 = Checksum::crc16ForModbus(m_currentRequest);

    out << quint8(crc16)
        << quint8(crc16 >> 8);

    m_currentRequest.resize(8);
    m_mutex.unlock();
}

int DataProcessThread::rq_actions(int idx, bool isWait)
{
    m_mutex.lock();
    QPair<int, bool> pair ;
    pair.first = static_cast<GripperCommand>(idx);
    pair.second = isWait;
    m_queue.enqueue(pair);
    m_mutex.unlock();

    if(isWait) {
        return m_eventloop.exec(QEventLoop::ExcludeUserInputEvents|
                                   QEventLoop::ExcludeSocketNotifiers);
    }
    return 0;
}
