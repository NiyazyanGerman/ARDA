#include "JsonDataProcessorHTTP.h"

JsonDataProcessorHTTP::JsonDataProcessorHTTP() {}

QString JsonDataProcessorHTTP::getFullPath(const QString &fileName)
{
    QFile file("StructFS.json");
    if(!file.open(QIODevice::ReadOnly))
    {

    }

    QByteArray bArr = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(bArr);

    QJsonArray arr= doc.array();

    for(const QJsonValue& v : arr)
    {
        if(!v.isObject()) continue;

        QJsonObject obj = v.toObject();

        if(obj["p"].toString().contains(fileName))
        {
            if(obj["t"].toString() == "f")
                return obj["p"].toString();
            else
                continue;
        }
        continue;
    }

    return "";
}

QString JsonDataProcessorHTTP::getFullPath()
{
    QFile file("StructFS.json");
    if(!file.open(QIODevice::ReadOnly))
    {

    }

    QByteArray bArr = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(bArr);

    QJsonArray arr= doc.array();

    for(const QJsonValue& v : arr)
    {
        if(!v.isObject()) continue;

        QJsonObject obj = v.toObject();

        if(obj["t"].toString() == "f")
            return obj["p"].toString();
        else
            continue;

    }

    return "";
}
