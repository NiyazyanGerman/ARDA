#ifndef DATASCHEDULE_H
#define DATASCHEDULE_H

#include <QObject>
#include <QList>
#include<QString>
#include<QMap>
#include<QTime>
#include <QHash>


static constexpr size_t MAX_LESSON = 8;

namespace Lesson {

enum class Type_Lesson {
    Lecture,
    Practice,
    Lab_Work,
    Seminar,
    Consultation,
    Exam,
    Credit_Test,
    Course_Project,
    Diploma_Project,
    Elective,
    Self_Study,
    Colloquium
};

const QList<QString> lessonTypes = {
    "Лекция",
    "Практика",
    "Лабораторная",
    "Семинар",
    "Консультация",
    "Экзамен",
    "Зачет",
    "Курсовая работа",
    "Дипломная работа",
    "Факультатив",
    "Самостоятельная работа",
    "Коллоквиум"
};

}// end namespace

struct Teacher {
    QString lastName; // фамилия
    QString firstName; // имя
    QString middleName; // отчество
    Teacher() = default;

    Teacher(const QString& name,const QString& LastName,const QString& MiddleName)
        : firstName(name),lastName(LastName),middleName(MiddleName)
    {

    }
    bool operator==(const Teacher& other) const {
        return lastName == other.lastName
               && firstName == other.firstName
               && middleName == other.middleName;
    }

    QString getFullName() const
    {
        return firstName + " " + lastName + " " + middleName;
    }

    int size() const{
        return QString(firstName+lastName+middleName).size();
    }

    bool operator< (const Teacher& otherTeacher) const
    {
        return this->size() < otherTeacher.size();
    }

    Teacher create(const QString name,const QString last,const QString midle = "")
    {
        this->firstName = name;
        this->middleName = midle;
        this->lastName = last;

        return Teacher(name,last,midle);
    }

    bool isEmpty() const {
        return firstName.isEmpty() || lastName.isEmpty();
    }

};


inline size_t qHash(const Teacher &t, size_t seed = 0)
{
    return qHash(t.lastName, seed)
    ^ qHash(t.firstName, seed << 1)
        ^ qHash(t.middleName, seed << 2);
}


struct DataSchedule
{
    typedef QMap<int, QPair<QTime,QTime>> DataMapLessonTime;
    DataSchedule() = default;

    DataMapLessonTime intanceTimeLessonDefault();
public:

    QSet<QString> cabinets;
    QSet<Teacher> Teachers;
    QSet<QString> LessonsName;
    DataMapLessonTime lessonsTime;
    QSet<QDate> datePair;
    QMap<Teacher,QList<QString>> hintsTeachers;

public:

    QString currentLessonName;
    Teacher currentTeacher;
    QString currentCabinet;
    QString currentLessonType;
    QDate currentDate;
    int currentPair;
    QTime currentStartTime;
    QTime currentEndTime;
};

#endif // DATASCHEDULE_H
