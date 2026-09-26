#include "DataWritterStudent.h"
#include<QDir>
#include"../Validator.h"
#include"../LogicOperation.h"
#include"../FileManagerCore/managerfs.h"
DataWritterStudent::DataWritterStudent() {}

bool DataWritterStudent::writeSimple(const StudentStructSimple &student)
{
    // new func

    const auto& [passes,passesYB] = passesPair(student);

    QString dir("Группа_"+student.Group);
    ManagerWorker c;
    c.CreateDir(".",dir);
    QString filePath = dir + "/" + student.Group+"_"+student.LessonName + ".json";

    QJsonArray arr;
    QFile file(filePath);

    if(file.size() != 0)
    {
        arr = LogicOperation::rewriteFile(file);
    }

  //  try {
        Validator::isFileValid(file, ModeValidator::WriteFile);
//        file.close();
    // } catch (const std::invalid_argument& e) {
    //     return false;
    // }

    QJsonObject obj;
    obj["ФИО"] = student.FullName;
    obj["Группа"] = student.Group;
    obj["№"] = student.ID;
    obj["Преподователь"] = student.TeacherName;
    obj["Название Предмета"] = student.LessonName;

    obj["Оценки"] = student.grades.join(", ");
    QStringList marksStrings;
    for(const QChar& c : student.marks) {
        marksStrings.append(QString(c));
    }

    obj["Марки"] = marksStrings.join(", ");
    if(student.averageBall.has_value())
        obj["Ср. Балл"] = student.averageBall.value();
    else
        obj["Ср. Балл"] = "н/a";

    obj["Кол-во часов"] = student.countPair*2;
    obj["Кол-во Пар"] = student.countPair;
    obj["Прогулы НЕУВ"] = passes;
    obj["Прогулы УВ"] = passesYB;


    arr.append(obj);


    QJsonDocument doc(arr);
    file.write(doc.toJson());
    file.close();

    return true;
}


QPair<double, double> DataWritterStudent::passesPair(const StudentStructSimple &studet)
{
    int passes = 0;
    int passesYB = 0;

    for(const auto& c : studet.marks)
    {
        if(c == "н" || c == "Н") passes+=2;
        else passesYB+=2;
    }
    return {passes,passesYB};
}


bool DataWritterStudent::writeSimplePDF(const QString &file_Json)
{
    // QFile file(file_Json);
    // try {
    //     Validator::isFileValid(file, ModeValidator::ReadFile);
    //     file.close();
    // } catch (const std::inv  alid_argument& e) {
    //     return false;
    // }

    PrinterData();

    return true;
}

QString DataWritterStudent::SetHeaderDocument()
{
    return "<tr><th>ID</th><th>ФИО</th><th>Преподаватель</th><th>Группа</th><th>Название предмета</th><th>Пропусков по ув.</th><th>Пропусков по неув.</th><th>Оценка за зачет</th></tr>";
}
QString DataWritterStudent::SetNameDocument()
{
    return QString("<h2 align='center'>Отчёт по предмету%1</h2>").arg("Алгебра"); // исправить
}


QString DataWritterStudent::setFormat(const QJsonObject &obj)
{
    QString id = obj["№"].toString();
    QString fio = obj["ФИО"].toString();
    QString group = obj["Группа"].toString();
    QString nameLesson = obj["Название Предмета"].toString();
    double gradeTest = obj["Ср. Балл"].toDouble();
    QString TeacherName=obj["Преподователь"].toString();
    int passes = obj["Прогулы НЕУВ"].toInt();
    int passesYB = obj["Прогулы УВ"].toInt();



    QString html = QString("<tr>"
                           "<td>%1</td>"
                           "<td>%2</td>"
                           "<td>%3</td>"
                           "<td>%4</td>"
                           "<td>%5</td>"
                           "<td>%6</td>"
                           "<td>%7</td>"
                           "<td>%8</td>"
                           "</tr>")
                       .arg(id, fio, TeacherName,group,nameLesson,QString::number(passes), QString::number(passesYB),QString::number(gradeTest));

    return html;

}

