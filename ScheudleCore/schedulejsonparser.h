#ifndef SCHEDULEJSONPARSER_H
#define SCHEDULEJSONPARSER_H
#include<QString>
#include"dataschedule.h"
#include<QMap>
#include<QTime>
#include<QPair>
#include<QSet>
#include"dataschedule.h"
#include<QFile>
#include"../LogicOperation.h"

class ScheduleJsonParser
{
public:
    ScheduleJsonParser();
public:
    bool setDataLessonTime(const QString &filename, const QMap<int,QPair<QTime,QTime>>& data);
    bool setDataCabinets(const QString &filename,const QSet<QString>& data);
    bool setLessonNameCabinets(const QString& filename,const QSet<QString>& data);
    bool setHintsTeachesrLessonName(const QString& filename,const QMap<Teacher,QList<QString>>& hints);
    QJsonObject parseScheudleToJson(const DataSchedule* scheudle);

    /**
 * Извлекает массив из JSON файла
 * Читает файл парсит JSON ищет первый массив (он и так 1)
 * Его данные сохраняет в массив
 * Если файл не существует или нет массива возвращает пустой массив
 * Удаляет оригинальный файл после чтения
 */
    static QJsonArray getArrayFromJsonFile(QFile& file);


    template<typename ContTeachers>
    bool setDataTeachers(const QString &filename, const ContTeachers &data)
    {
        QFile file(filename);

        QJsonArray arr = getArrayFromJsonFile(file);

        LogicOperation::ValidFile(file,ModeValidator::WriteFile);

        for(const auto& teacher : data)
        {
            QJsonObject obj;
            obj["Имя"] = teacher.firstName;
            obj["Фамилия"] = teacher.lastName;
            obj["Отчество"] = teacher.middleName;
            arr.append(obj);
        }
        QJsonObject MainObj;

        if(arr.isEmpty()) return false;

        MainObj["Учителя"]=arr;

        QJsonDocument doc(MainObj);
        file.write(doc.toJson());
        return true;
    }

};

#endif // SCHEDULEJSONPARSER_H
