#ifndef MANAGERTABLESDB_H
#define MANAGERTABLESDB_H

#include <QObject>
#include<QNetworkAccessManager>
#include"../ActiveSession.h"
class ManagerTablesDB : public QObject
{
    Q_OBJECT
public:
    explicit ManagerTablesDB(QObject *parent = nullptr);

signals:

private:
    //std::unique_ptr<ActiveSession>
};

#endif // MANAGERTABLESDB_H
