#include "ConnectionInfoHTTP.h"

//ConnectionInfoHTTP::ConnectionInfoHTTP() {}

QString ConnectionInfoHTTP::ip() const
{
    return m_ip;
}

void ConnectionInfoHTTP::setIp(const QString &newIp)
{
    if (m_ip == newIp)
        return;
    m_ip = newIp;
   // emit ipChanged();
}

quint16 ConnectionInfoHTTP::port() const
{
    return m_port;
}

void ConnectionInfoHTTP::setPort(quint16 newPort)
{
    if (m_port == newPort)
        return;
    m_port = newPort;
    //emit portChanged();
}
