#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include <QThread>
#include <QMutex>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QWebEngineView>
#include <QEventLoop>

class ProcessThread : public QThread
{
    Q_OBJECT

public:
    ProcessThread(QObject *parent = 0);
    ~ProcessThread();

    ///
    /// \brief initialCameraAddress()
    ///
    /// \param addr : camera IP address
    ///
    void initialCamera(const QString &addr ,QNetworkAccessManager *manager);


    ///
    /// \brief res_position()
    /// \param x
    /// \param y
    /// \param wait : block or not
    /// \return   0-> sucess , not 0 failed
    ///
    int res_position(QString &x, QString &y,
                     QString &z, bool wait = true);

    ///
    /// \brief recSickCharacter()
    ///
    /// \param recSickCharacter : query name and camera analysed character
    ///
    void recSickCharacter(QMap<QString, QString> &recSICKCharacter);

    ///
    /// \brief res_singleData()
    /// \param type :
    /// \param data :
    /// \param wait : block or not
    /// \return  0-> sucess , not 0 failed
    ///
    int res_singleData(const QString &type, QString &data, bool wait = true);
    void readyPoint(QString &ready_point_x, QString &ready_point_y);
    double m_dataFilter;
    QString objectShape;
    QString m_jointRadian;

signals:
    void errors(const QString &s);
    void cameraResult(const QByteArray &data);
    void cameraGetPositionX(const QString &positionx);
    void cameraGetPositionY(const QString &positiony);
    void cameraCharacter(const QMap<QString, QString> &character);

protected:
    void run();

private:
    void analyseResult(const QByteArray &array, QMap<QString, QString> &result);

    QMutex m_mutex;
    QString m_address;
    QString m_selectObjectFlag;
    bool m_quit;
    bool m_threadDataFinish;

    QNetworkAccessManager *m_netManger;

    QEventLoop  m_eventLoop;
    QByteArray m_reciveData;
    QMap<QString, QString>  m_result;
    QMap<QString, QString>  m_recSickCharacter;

};

#endif // PROCESSTHREAD_H
