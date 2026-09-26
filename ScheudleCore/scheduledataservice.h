#ifndef SCHEDULEDATASERVICE_H
#define SCHEDULEDATASERVICE_H

#include <QObject>
#include"dataschedule.h"
#include<algorithm>
#include<ranges>

class ScheduleDataService : public QObject
{
    Q_OBJECT
public:
    ScheduleDataService();

    enum class SortTypeTeachers {
        Default = 0,
        Length,
        Lexicographical,
        FirstName,
        LastName,
        MiddleName
    };

public slots:
    QSet<QString> GetCabinetsJson();
    QSet<QString> GetLessonNameJson();
    QMap<int,QPair<QTime,QTime>> GetLessonTimeJson();
    QSet<Teacher> GetTeacherNameJson();

    bool sortTeacherWithParam(QList<Teacher> &teachers, SortTypeTeachers typeSorting);

private:
    std::unique_ptr<DataSchedule> schedule;


};

#endif // SCHEDULEDATASERVICE_H
