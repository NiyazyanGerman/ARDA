#ifndef DELETEUPDATEOBJECTSHTTP_H
#define DELETEUPDATEOBJECTSHTTP_H

#include <QObject>

class DeleteUpdateObjectsHTTP : public QObject
{
    Q_OBJECT
public:
    explicit DeleteUpdateObjectsHTTP(QObject *parent = nullptr);

    void deleteObject(const QString& pathDelete);

signals:
};

#endif // DELETEUPDATEOBJECTSHTTP_H
