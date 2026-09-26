#ifndef MANAGERJSONLESSON_H
#define MANAGERJSONLESSON_H

#include <QObject>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonDocument>
#include<QFile>
#include"../AdapterTables.h"
#include"../../Validator.h"

class ManagerJsonLesson : public QObject
{
    Q_OBJECT
public:
    explicit ManagerJsonLesson(QObject *parent = nullptr);
 ~ManagerJsonLesson();
signals:
 void WriteSucceful();
public:

template<typename T>
bool ParserAdapter(std::vector<std::unique_ptr<T>> &lessonsData, QFile& file)
{
    if(lessonsData.empty()) return false;


    QJsonArray mainArray;

    for (const auto& item : lessonsData) {
        if (!item) continue;

        QJsonObject currObject = item->toJson();

        mainArray.append(currObject);
    }

    QJsonDocument doc(mainArray);

    try {
        if (file.size() != 0) {
            Validator::isFileValid(file, ModeValidator::DeleteWrite);
        } else {
            Validator::isFileValid(file, ModeValidator::WriteFile);
        }
    } catch (const std::logic_error& err) {
        qDebug() << "ParserAdapter: File validation failed:" << err.what();
        return false;
    }

    file.write(doc.toJson());
    file.close();
    return true;
}

std::vector<std::unique_ptr<LessonAdapter> > LoadAdapter(QFile& file);

};

#endif // MANAGERJSONLESSON_H
