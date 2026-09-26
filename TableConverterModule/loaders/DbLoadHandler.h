#ifndef DBLOADHANDLER_H
#define DBLOADHANDLER_H

#include <QObject>
#include"../../AutoSaverLesson/AdapterTables.h"

//template<typename T>
class DbLoadHandler : public QObject
{
public:
    explicit DbLoadHandler(QObject *parent = nullptr);




public slots:


private:
   // std::vector<std::unique_ptr<T>> DataTable;
};

#endif // DBLOADHANDLER_H
