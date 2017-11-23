#ifndef SICK_PIMXPLUGINWIDGET_H
#define SICK_PIMXPLUGINWIDGET_H

#include <QWidget>
#include <QUrl>
#include <QSet>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTableWidget>
#include <QWebView>

class CameraControl;

namespace Ui {
class SICK_PIMxPluginWidget;
}

class QTimer;
class QWebEngineView;

class SICK_PIMxPluginWidget : public QWidget
{
    Q_OBJECT
public:
    SICK_PIMxPluginWidget(CameraControl *camera, QWidget *parent = 0);

    ~SICK_PIMxPluginWidget();


private slots:
    void on_pbn_connect_clicked();
    void on_pbn_showHideImage_clicked();
    void on_rbn_Overlay_clicked();
    void on_rbn_RawData_clicked();
    void on_rbn_SimpleOverlay_clicked();
    void on_pbn_snapShot_clicked();
    void on_tbn_imageSave_clicked();
    void slotReplyPixmapLoad(QNetworkReply* reply);
    void on_pbn_toolOffsetSave_clicked();
    void on_pbn_compensation_clicked();

    void on_pbn_searchSickCharacter_clicked();

    void on_pbn_initCamera_clicked();

//    void on_pbn_test_clicked();

    void on_pbn_delete_clicked();

//    void on_pbn_Save_clicked();
    void slot_recvRealTimeRoadPoint(QString Data);

    void on_pbn_teach_check1_clicked();

    void on_pbn_teach_check2_clicked();

    void on_pbn_teach_check3_clicked();

    //void on_pbn_teach_check4_clicked();

private:
    void slotTimeout();
    void slotUpdateCameraDataPage();
    void initScriptProgect();
    void camera_1ScriptFile();
    void camera_2ScriptFile();
    int searchCharacter();
    void soltRefreshCameraCharacter(QMap <QString, QString> character);

    void initWgt();

public:
    void setPluginInfomation(QString organization,
                             QString whatsThis,
                             QString name,
                             QString author,
                             QString version);

private:
    Ui::SICK_PIMxPluginWidget *ui;

    CameraControl *m_camera;
    QWebEngineView *m_view;
    QTimer *m_timer;
    QNetworkAccessManager *m_netManger;
    QTableWidget *m_inputMessage;

    QUrl m_imageUrl;
    QString m_saveDirectory;
    QString m_robotReadyHeight;
    QString m_robotCatchHeight;
    QList <QString> m_searchName;
    QList <int> m_deleteCharacterPosition;

    QWebView *m_pluginView;

};

#endif // SICK_PIMXPLUGINWIDGET_H














































