#include "ApiHandler.h"
#include<QTimer>
#include<QUrl>

ApiHandler::ApiHandler() {

    network_manager = new QNetworkAccessManager();
}

QVariant ApiHandler::execute(const std::string &path_api, const QJsonObject &isPostBidy)
{
    QNetworkRequest request(QUrl{"https://localhost:1123" + QString::fromStdString(path_api)});
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    qDebug() << request.url();


    qDebug() << request.url();

    if(!isPostBidy.empty()){
        QJsonDocument doc(isPostBidy);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = network_manager->post(request, QJsonDocument(isPostBidy).toJson());

        reply->setProperty("request_id", ++requestCounter);

        connect(reply, &QNetworkReply::finished, this, &ApiHandler::onReplyFinisched);
        connect(reply, &QNetworkReply::errorOccurred, this, &ApiHandler::onReplyError);

        QTimer::singleShot(30000, [reply]() {
            if (reply && reply->isRunning()) {
                reply->abort();
                reply->deleteLater();
                qDebug() << "Timeout!";
            }
        });


    }
 return QVariant();
}

void ApiHandler::onReplyFinisched()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    int requestId = reply->property("request_id").toInt();

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        emit requestSuccess(doc.object(), requestId);
    } else {
        emit requestError(reply->errorString(), requestId);
    }
    reply->deleteLater();
}

void ApiHandler::onReplyError(QNetworkReply::NetworkError code)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int requestId = reply->property("request_id").toInt();
    emit requestError(reply->errorString(), requestId);
    reply->deleteLater();
}
