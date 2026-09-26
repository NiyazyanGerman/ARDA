#ifndef UPLOADFILEHTTP_H
#define UPLOADFILEHTTP_H

#include <QObject>
#include"ConnectionInfoHTTP.h"

#include"ApiService.h"
class UploadFileHTTP : public QObject
{
    Q_OBJECT
public:
    UploadFileHTTP(std::shared_ptr<ConnectionInfoHTTP> info);

    void UploadFile(const QString& filePath, const QString& savePath, bool Move);
private:
    std::shared_ptr<ConnectionInfoHTTP> infoServer;

    QString ip;

    std::unique_ptr<ApiService> apiSERVER;

signals:
    void UploadProgress(qint64 r, qint64 t);
    void UplaodFinished(const QString& response);
};

#endif // UPLOADFILEHTTP_H
