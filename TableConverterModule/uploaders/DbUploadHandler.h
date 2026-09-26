#ifndef DBUPLOADHANDLER_H
#define DBUPLOADHANDLER_H

#include <QObject>

class DbUploadHandler : public QObject
{
    Q_OBJECT
public:
    explicit DbUploadHandler(QObject *parent = nullptr);

signals:
};

#endif // DBUPLOADHANDLER_H
