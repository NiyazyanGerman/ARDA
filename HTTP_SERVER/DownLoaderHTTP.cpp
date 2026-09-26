    #include "DownLoaderHTTP.h"
#include<QFileInfo>
#include<QUrlQuery>
#include<QDir>
#include<QEventLoop>
#include<QMutexLocker>

DownLoaderHTTP::DownLoaderHTTP(std::shared_ptr<ConnectionInfoHTTP> infoServer)
{
    manager = std::make_unique<QNetworkAccessManager>();
    apiSERVER = std::make_unique<ApiService>();
    info = infoServer;

    port = info->port();
    ipV4 = info->ip();

}

bool DownLoaderHTTP::DownloadFile(const QString &path, const QString &savePath)
{
    url.setScheme("https");
    url.setHost(info->ip());
    url.setPort(info->port());
    url.setPath("/api/DownloadFile");

    QUrlQuery query;
    query.addQueryItem("path", path);
    url.setQuery(query);

    qDebug() << url << " ef" << fileName << " ef " << SavePath_;
    if(!url.isValid()) return false;

    Download(QFileInfo(path).fileName(),savePath);

    return true;
}
void DownLoaderHTTP::Download(const QString& filename, const QString& savePath)
{

    QString fullPath = savePath + "/" + filename;


    auto file = QSharedPointer<QFile>::create(fullPath);


    if(!file->open(QIODevice::WriteOnly)) return;

    currentsFiles.append(file);


    qDebug()<<file.get();

    connect(apiSERVER.get(), &ApiService::downloadRead,this, [file](const QByteArray& arr){
        file->write(arr);
    });

    connect(apiSERVER.get(), &ApiService::downloadProgress,this, [this](qint64 r, qint64 t){
        emit downloadProgress(r,t);

    });


    connect(apiSERVER.get(), &ApiService::downloadFinished,
            this, [file,this](){
        file->close();
        currentsFiles.removeOne(file);
        emit finish();

    });

    apiSERVER->getByteArrayForDownLoading(url);
}
