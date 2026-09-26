#ifndef UTILSHTTP_H
#define UTILSHTTP_H

#include <QObject>
#include<QUrl>
class UtilsHTTP : public QObject
{
    Q_OBJECT
public:
    explicit UtilsHTTP(QObject *parent = nullptr);

   // QByteArray cryptedByte(const QByteArray& bytes);

    QString backDirServerConstructPath(const QString& curentPath);

    QString getRegisterLogin();

signals:
};

#endif // UTILSHTTP_H
