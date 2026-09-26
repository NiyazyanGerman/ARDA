#ifndef JSONDATAPROCESSORHTTP_H
#define JSONDATAPROCESSORHTTP_H
#include<QString>
#include<QJsonArray>
#include<QJsonObject>
#include<QFile>
#include<QByteArray>

class JsonDataProcessorHTTP
{
public:
    JsonDataProcessorHTTP();

    QString getFullPath(const QString& fileName);

    QString getFullPath();
};

#endif // JSONDATAPROCESSORHTTP_H
