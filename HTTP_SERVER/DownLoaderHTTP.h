#ifndef DOWNLOADERHTTP_H
#define DOWNLOADERHTTP_H

#include <QObject>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>
#include<optional>
#include<memory>
#include"ConnectionInfoHTTP.h"
#include"ApiService.h"
#include<QMutex>

class DownLoaderHTTP : public QObject
{
Q_OBJECT

public:
    DownLoaderHTTP(std::shared_ptr<ConnectionInfoHTTP> infoServer);

    bool DownloadFile(const QString& path,const QString& savePath);

private:

    void Download(const QString &filename, const QString &savePath);
    //void Download(int);

    QMutex m;
    QString SavePath_;
    QString ipV4;
    qint16 port;
    QUrl url;
    QString fileName;

    std::unique_ptr<QNetworkAccessManager> manager;
    std::shared_ptr<ConnectionInfoHTTP> info;
    std::unique_ptr<ApiService> apiSERVER;
   QList<QSharedPointer<QFile>> currentsFiles;

signals:

    void downloadProgress(qint64 received, qint64 total);
    void finish();
    void startD(qint64 total);
};

#endif // DOWNLOADERHTTP_H
