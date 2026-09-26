#include "ActiveSession.h"
#include<QRandomGenerator>
ActiveSession::ActiveSession(QObject *parent)
    : QObject{parent}
{}

ActiveSession *ActiveSession::instance()
{
    static ActiveSession* session = new ActiveSession();
    return session;
}

void ActiveSession::setExpiresAt()
{
    m_loginTime = QDateTime::currentDateTime();
    m_expiresTime = m_loginTime.addSecs(8 * 60 * 60);
}

const QString& ActiveSession::getToken()
{
    return token;
}

void ActiveSession::initSession(const std::string &fullName, const std::string &email)
{
    m_fullName = fullName;
    m_email = email;
    m_loginTime = QDateTime::currentDateTime();

    ActiveSession::setExpiresAt();
}

void ActiveSession::generatedToken(int len)
{

    std::unique_ptr<QByteArray> arr =std::make_unique<QByteArray>();
    arr->resize(len / 2);

    QRandomGenerator::system()->fillRange(
        reinterpret_cast<quint32*>(arr->data()),
        arr->size() / sizeof(quint32)
        );

    token = QString::fromUtf8(arr->toHex());

}


