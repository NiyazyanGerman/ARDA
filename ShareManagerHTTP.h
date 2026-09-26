#ifndef SHAREMANAGERHTTP_H
#define SHAREMANAGERHTTP_H

#include <QObject>
#include<QFileSystemModel>
#include<QFileInfoList>

class ShareManagerHTTP : public QObject
{
    Q_OBJECT
public:
    ShareManagerHTTP(QFileSystemModel* fs, QObject* object = nullptr);
    ~ShareManagerHTTP() override = default;

private:
    QFileSystemModel* fs_;

    QString IP;
    QString Port;
    QString loginpasswordAdministrator, passwordAdministrator;

};


class ManagerFile
{
public:

    virtual ~ManagerFile() = default;

    ManagerFile(QFileSystemModel* fs) : fs_(fs)
    {

    }


    virtual moveFiles(const QFileInfoList& filesMovesList,const QString& movePath) = 0;
    virtual copyFiles(const QFileInfoList& filesCopyList,const QString& CopyPath) = 0;
    virtual removeFiles(const QFileInfoList& filesRemovesList) = 0;


protected:
    QFileSystemModel* fs_;

};

#endif // SHAREMANAGERHTTP_H
