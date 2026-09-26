#ifndef APISERVICE_H
#define APISERVICE_H

#include <QObject>
#include<QNetworkAccessManager>
#include<QFileSystemModel>
#include<QSortFilterProxyModel>
#include<QFileInfoList>
#include<QNetworkReply>
#include<QByteArray>
#include"ConnectionInfoHTTP.h"


class ApiService : public QObject
{
    Q_OBJECT
public:
    //ApiService(QObject* object = nullptr);
    ApiService(QObject* object = nullptr);
    ~ApiService() override = default;

    void getByteArrayForDownLoading(const QUrl &url);

    void GetListFileSystemServer(const QUrl &url,const QString& path);

    void GetInfoFile(const QUrl &url);

    void PostUploadFile(QFile* file, const QUrl &url);

    void GetDeleteObject(const QUrl& url);


    void GetBackDir(const QString& currentPath,std::shared_ptr<ConnectionInfoHTTP> infoServer);
private:
    std::unique_ptr<QNetworkAccessManager> manager;
    QUrl url_;
signals:

    void downloadProgress(qint64 received, qint64 total);
    void downloadRead(const QByteArray& data);
    void downloadFinished();
    void startDownload(qint64 total);

    void UploadFileProgress(qint64 received, qint64 total);
    void UploadFileFinished(const QString& response);

    void infoFileReady(const QByteArray& arr);

    void finishedListFile();
};




#endif // APISERVICE_H
