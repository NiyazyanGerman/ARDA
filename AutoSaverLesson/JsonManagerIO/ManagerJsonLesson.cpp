#include "ManagerJsonLesson.h"
#include<QJsonDocument>
#include<QJsonValue>
#include<QJsonObject>

ManagerJsonLesson::ManagerJsonLesson(QObject *parent)
    : QObject{parent}
{}

ManagerJsonLesson::~ManagerJsonLesson()
{

}

std::vector<std::unique_ptr<LessonAdapter> > ManagerJsonLesson::LoadAdapter(QFile &file)
{
    try
    {
        if(file.size() != 0){
            Validator::isFileValid(file,ModeValidator::ReadFile);
        }
        else{
            return {};
        }

    }
    catch(const std::logic_error& err)
    {
        return {};
    }


    QByteArray arr = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(arr);
    QJsonArray arrayJson(doc.array());
    std::vector<std::unique_ptr<LessonAdapter>> lessonAdapters;

    for(const QJsonValue& value : arrayJson)
    {
        if(!value.isObject()) continue;

        QJsonObject obj = value.toObject();


        auto lesson = std::make_unique<LessonAdapter>();
        lesson->id = obj["ID"].toString();
        lesson->fio = obj["fullName"].toString();
        lesson->group = obj["Group"].toString();
        lesson->columnCount = DEFAULT_TABLES + obj["Pair"].toInt();
        lesson->lessonName = obj["NameLesson"].toString();

        QJsonArray dates = obj["Dates"].toArray();
        for(const QJsonValue& date : dates)
        {
            if(!    date.isObject()){
                continue;
            }

            QJsonObject dateObj = date.toObject();
            lesson->daysInfo[dateObj["Date"].toString()]=dateObj["Marks"].toString();

        }

        lessonAdapters.push_back(std::move(lesson));

    }


    return lessonAdapters;

}
