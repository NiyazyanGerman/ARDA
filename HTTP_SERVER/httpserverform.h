#ifndef HTTPSERVERFORM_H
#define HTTPSERVERFORM_H

#include <QWidget>
#include<memory>
#include<QSortFilterProxyModel>
#include<QNetworkAccessManager>
#include<QStandardItemModel>
#include"ConnectionInfoHTTP.h"
#include"DownLoaderHTTP.h"
#include"JsonDataProcessorHTTP.h"
#include"ApiService.h"
#include"UploadFileHTTP.h"
#include"utilshttp.h"
#include<QElapsedTimer>
#include<QProgressBar>
#include<QListWidgetItem>
#include<QLabel>

class MainWindow;


namespace Ui {
class HTTPserverForm;
}

class HTTPserverForm : public QWidget
{
    Q_OBJECT

public:
    explicit HTTPserverForm(MainWindow* main,QWidget *parent = nullptr);
    ~HTTPserverForm();
     void init();
private slots:

    void ConstructTreeView();



    void on_btnRegestration_clicked();

private:


    struct DownloadTask {
        QListWidgetItem *item;
        QProgressBar *progressBar;
        QLabel *taskLabel;
        QElapsedTimer timer;
        qint64 lastBytes;
        QString fileId;
    };

    Ui::HTTPserverForm *ui;
    MainWindow* mainWin;
    QHash<DownLoaderHTTP*,QProgressBar*> downloadInfo;
    QHash<UploadFileHTTP*,QProgressBar*> uploadInfo;

    QNetworkAccessManager* manager;
    QStandardItemModel* modelTree;


    std::shared_ptr<ConnectionInfoHTTP> infoServer;
    std::unique_ptr<UtilsHTTP> utilsHelperHttp;
    std::unique_ptr<JsonDataProcessorHTTP> jsonManager;
    std::unique_ptr<ApiService> apiHelper;

   void startOperation(const QString &ID,const QString& task, qint64 bytesTotal);
    void removeDataInfo(const QString& ID);
    void updateProgress(const QString &ID, qint64 bytesReceived, qint64 totalSize);
};

#endif // HTTPSERVERFORM_H
