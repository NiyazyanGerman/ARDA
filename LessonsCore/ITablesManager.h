#ifndef ITABLESMANAGER_H
#define ITABLESMANAGER_H

#include <QObject>
#include"../FileChooicer.h"

class ITablesManager : public QWidget
{
    Q_OBJECT
public:


    explicit ITablesManager(QWidget *parent = nullptr);

    void PrinterData();

protected:
    virtual QString SetNameDocument() = 0;
    virtual QString SetHeaderDocument() = 0;
    virtual QString setFormat(const QJsonObject& obj) = 0;
signals:

private:
    void init();
    std::unique_ptr<FileManager> fileChoicer;
};

#endif // ITABLESMANAGER_H
