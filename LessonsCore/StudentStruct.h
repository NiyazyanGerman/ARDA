 #ifndef STUDENTSTRUCT_H
#define STUDENTSTRUCT_H

#include <QObject>
#include<optional>

class StudentStructSimple
{
public:
    StudentStruct();
    // StudentStruct(QString FullName, QString UniqueID,
    //               QString LessonName, QString GroupStudent,QString grades,QString marks);

    QString ID;
    QString Group;
    QString FullName;
    QString LessonName;
    int countPair;
    QStringList grades;
    QList<QChar> marks;
    QString recordBook;
    QString TeacherName;
    std::optional<double> averageBall;
    QString autoTest;
    int procentPasses;

    // void createStudent(QString FullName, QString UniqueID,
    //                    QString LessonName, QString GroupStudent,QString grades,QString marks);

};

class StudentStructHigh : public StudentStructSimple
{
public:
    StudentStruct();
    StudentStruct(QString FullName, QString UniqueID, int courseStudent,
        QString FacultyStudent, QString GroupStudent);

    QString recordBook;
    QString ID;

    int course;
    QString Faculty;
    QString Group;
    QString FullName;

    void createStudent(QString FullName, QString UniqueID, int courseStudent,
                       QString FacultyStudent, QString GroupStudent);


};


#endif // STUDENTSTRUCT_H
