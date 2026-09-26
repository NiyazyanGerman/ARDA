#ifndef DATAWRITTERSTUDENT_H
#define DATAWRITTERSTUDENT_H

#include <QObject>
#include"StudentStruct.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include"ITablesManager.h"
class DataWritterStudent  : public ITablesManager
{


public:
    DataWritterStudent();

public slots:
    bool writeHigh(const QList<StudentStructHigh>& students);
    bool writeHigh(const StudentStructHigh& student);


    bool writeSimple(const QList<StudentStructSimple>& students);
    bool writeSimple(const StudentStructSimple& student);
    bool writeSimplePDF(const QString& file_Json);

    // QString SetHeaderDocument() override;
    // QString SetNameDocument() override;
    // QString setFormat(const QJsonObject &obj) override;
private:
    QPair<double,double> passesPair(const StudentStructSimple &studet);
protected:
    QString setFormat(const QJsonObject &obj);
    QString SetHeaderDocument();
    QString SetNameDocument();
};


#endif // DATAWRITTERSTUDENT_H
