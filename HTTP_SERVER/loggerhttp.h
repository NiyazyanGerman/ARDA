#ifndef LOGGERHTTP_H
#define LOGGERHTTP_H

#include <QObject>

class LoggerHTTP : public QObject
{
    Q_OBJECT
public:
    explicit LoggerHTTP(QObject *parent = nullptr);

signals:
};

#endif // LOGGERHTTP_H
