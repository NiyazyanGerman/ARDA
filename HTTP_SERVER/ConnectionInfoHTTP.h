#ifndef CONNECTIONINFOHTTP_H
#define CONNECTIONINFOHTTP_H
#include<QString>
#include<QObject>

class ConnectionInfoHTTP : public QObject {
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)

    QString m_ip;
    quint16 m_port = 8443;
    QString login;
    QString password;

   auto operator<=>(const ConnectionInfoHTTP&) const = delete;

public:
    QString ip() const;
    void setIp(const QString &newIp);

    quint16 port() const;
    void setPort(quint16 newPort);

signals:
    void ipChanged();
    void portChanged();
};
#endif // CONNECTIONINFOHTTP_H
