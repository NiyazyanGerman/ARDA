#ifndef MANAGERPERMISSIONS_H
#define MANAGERPERMISSIONS_H

#include <QObject>

class ManagerPermissions : public QObject
{
    Q_OBJECT
public:
    explicit ManagerPermissions(QObject *parent = nullptr);

signals:
};

#endif // MANAGERPERMISSIONS_H
