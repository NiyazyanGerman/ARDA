#ifndef ACTIVESESSION_H
#define ACTIVESESSION_H

#include <QObject>
#include<QTime>
#include<QDateTime>
#include<QString>

class ActiveSession : public QObject
{
    Q_OBJECT
    ActiveSession(const ActiveSession&) = delete;
    ActiveSession& operator=(const ActiveSession&) = delete;

public:
    explicit ActiveSession(QObject *parent = nullptr);
    static ActiveSession* instance();


    bool isTimeOutSession() const {
        return QDateTime::currentDateTime() >= m_expiresTime;
    }


    void generatedToken(int len = 64);
    const QString &getToken();


     void initSession(const std::string& fullName,const std::string& email);

    std::string getFullName() const {
        return m_fullName;
    }

    QDateTime getLoginTime() const {
        return m_loginTime;
    }

    QDateTime getExpiresTime() const {
        return m_expiresTime;
    }

    std::string getEmail() const {
        return m_email;
    }

private:
    QString token;
    std::string m_fullName;
    QDateTime m_loginTime;
    QDateTime m_expiresTime;
    std::string m_email;
    void setExpiresAt();

};

#endif // ACTIVESESSION_H
