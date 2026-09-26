#include "ApiService.h"
#include<QNetworkReply>
#include<QEventLoop>
#include<QUrlQuery>
#include"ConnectionInfoHTTP.h"
ApiService::ApiService(QObject *object)

{

    manager = std::make_unique<QNetworkAccessManager>();
}

void ApiService::getByteArrayForDownLoading(const QUrl&url)
{
    QEventLoop loop;
    QNetworkRequest request(url);
    qDebug() << url;

    auto reply = manager->get(request);


    qint64 totalSize = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();

    emit startDownload(totalSize);

    if(reply->error() == QNetworkReply::NoError) qDebug() << "ощибок нек";


    connect(reply,&QNetworkReply::downloadProgress,[this,reply](qint64 r, qint64 s){
        emit downloadProgress(r,s);
    });

    connect(reply, &QNetworkReply::readyRead, [reply,this]() {
        emit downloadRead(reply->readAll());

    });

    connect(reply,&QNetworkReply::finished,[this,reply,&loop](){
        emit downloadFinished();
        reply->deleteLater();
        loop.quit();
    });

    loop.exec();
}

void ApiService::GetListFileSystemServer(const QUrl& url,const QString& path)
{
    QUrlQuery query;
    query.addQueryItem("path",path);
    url.setQuery(query);

    QNetworkRequest request(url);

    auto reply = manager->get(request);

    QFile *file = new QFile("StructFS.json");
    if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file->deleteLater();
        reply->deleteLater();

    }


    connect(reply, &QNetworkReply::finished, [this, reply, file]() {
        if (reply->error() == QNetworkReply::NoError) {
            file->write(reply->readAll());
            file->close();
            qDebug() << "File saved, size:" << file->size();
            emit finishedListFile();
        } else {
            qDebug() << "Network error:" << reply->errorString();
            file->close();
            emit finishedListFile();
        }

        reply->deleteLater();
        file->deleteLater();
    });

}

void ApiService::GetInfoFile(const QUrl &url)
{
    QNetworkRequest request(url);
    auto reply = manager->get(request);

    if(reply->error() == QNetworkReply::NoError) qDebug() << "ощибок нек";

    connect(reply,&QNetworkReply::finished,this,[this,reply](){
       emit infoFileReady(reply->readAll());
        reply->deleteLater();
    });

}

void ApiService::PostUploadFile(QFile *file, const QUrl& url)
{

    QNetworkRequest request(url);

    qDebug() << file->size() << "size arr";

    request.setRawHeader("Content-Type", "application/octet-stream");

    auto reply = manager->post(request,file);

    connect(reply, &QNetworkReply::finished, this, [reply,file,this]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit UploadFileFinished(reply->readAll());
        } else {
            emit UploadFileFinished("Ошибка :" + reply->errorString());
        }
        reply->deleteLater();
        file->close();
        delete file;
    });

    connect(reply, &QNetworkReply::uploadProgress, [this](qint64 bytesSent, qint64 bytesTotal) {
        UploadFileProgress(bytesSent,bytesTotal);
    });
}

void ApiService::GetDeleteObject(const QUrl &url)
{

    QEventLoop loop;

    QNetworkRequest request(url);
    qDebug() << url << " get deleted";

    auto reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [reply,&loop]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "File deleted"
                        "";

        } else {
            qWarning() << "Ошибка при отправке файла:" << reply->errorString();
        }
        reply->deleteLater();

        loop.quit();
    });

    loop.exec();
}

void ApiService::GetBackDir(const QString &currentPath, std::shared_ptr<ConnectionInfoHTTP> infoServer)
{
    QEventLoop loop;
    QUrl url;
    url.setScheme("https");
    url.setHost(infoServer->ip());
    url.setPort(infoServer->port());
    url.setPath("/api/ListFile");

    QUrlQuery query;
    query.addQueryItem("path", currentPath);
    url.setQuery(query);
    qDebug() << "url = " << url;
    QNetworkRequest request(url);

    auto reply = manager->get(request);

    QFile *file = new QFile("StructFS.json");
    if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file->deleteLater();
        reply->deleteLater();
        return ;
    }

    connect(reply, &QNetworkReply::readyRead, [reply, file]() {
        file->write(reply->readAll());
    });

    connect(reply, &QNetworkReply::finished, [this, reply, file,&loop]() {
        file->close();
        qDebug()<<  file->size();

        loop.quit();
        reply->deleteLater();
        file->deleteLater();

    });

    loop.exec();

}




