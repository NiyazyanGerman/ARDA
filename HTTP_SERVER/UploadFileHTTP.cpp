#include "UploadFileHTTP.h"
#include<QHttpMultiPart>
#include<QHttpPart>
#include<QJsonObject>
#include<QJsonDocument>
#include<QSettings>

#include<QUrlQuery>

UploadFileHTTP::UploadFileHTTP(std::shared_ptr<ConnectionInfoHTTP> info)
    : infoServer(info)
{
    apiSERVER = std::make_unique<ApiService>();
    ip = "https://" + infoServer->ip() + ":" + QString::number(infoServer->port());

}

void UploadFileHTTP::UploadFile(const QString &filePath, const QString &savePath,bool Move)
{

    QFile* file = new QFile(filePath);
    QFileInfo fileName_(filePath);
    if(!file->open(QIODevice::ReadOnly))
    {
        return;
    }
    qDebug() << "вошел в загрузчик";
    // if(file.size() >= 104857600 )
    // {

    QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography", QSettings::NativeFormat);
    QString machineGuid = registry.value("MachineGuid").toString();

    machineGuid.remove("{").remove("}").trimmed();

    qDebug() << "HWID " << machineGuid;
    QUrl url = ip + "/api/UploadFile";
    QUrlQuery query;
    query.addQueryItem("filename",fileName_.fileName());
    query.addQueryItem("savePath",savePath);
    query.addQueryItem("userId",machineGuid);
    url.setQuery(query);

    connect(apiSERVER.get(),&ApiService::UploadFileProgress,this,[this](qint64 r, qint64 t){
        emit UploadProgress(r,t);
    });

    connect(apiSERVER.get(),&ApiService::UploadFileFinished,this,[this](const QString& response){
        emit UplaodFinished(response);
    });

    apiSERVER->PostUploadFile(file,url);

    if(Move)
       file->remove();

    return;
}
