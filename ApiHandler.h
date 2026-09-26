#ifndef APIHANDLER_H
#define APIHANDLER_H

#include<QVariant>
#include<QJsonObject>
#include<string>

#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>


class ApiHandler : public QObject
{
    Q_OBJECT
public:
    ApiHandler();

    QVariant execute(const std::string& path_api, const QJsonObject& isPostBidy = {});



private:
    QNetworkAccessManager* network_manager;
     int requestCounter = 0;
signals:
    void requestSuccess(const QJsonObject &data, int requestId);
    void requestError(const QString &error, int requestId);

private slots:
    void onReplyFinisched();
    void onReplyError(QNetworkReply::NetworkError code);
};

#endif // APIHANDLER_H
