#ifndef ADAPTERTABLES_H
#define ADAPTERTABLES_H

#include<QDate>
#include<QList>
#include<QString>
#include<QPair>
#include<QTableWidget>
#include<QJsonObject>
#include<QJsonValue>
#include<QJsonArray>

#define DEFAULT_TABLES 4

struct LessonAdapter {
    enum class TYPE_ADAPTER
    {
        Lesson = 1305,
        Record,
        Project,
        Course,
        Diploma
    };


    QString id;
    QString fio;
    QString group;
    QString lessonName;
    int rowCount;
    int columnCount;
    QMap<QString, QString> daysInfo;

    // LessonAdapter(const QString& id_,const QString& fio_, const QString& group_,const QString& LessonName_,const QPair<QString,QString>& days)
    //     : id(id_),fio(fio_),group(group_),lessonName(LessonName_){
    //         daysInfo[days.first]=days.second;
    // }

    // LessonAdapter(const QString& id_,const QString& fio_, const QString& group_,const QString& LessonName_)
    //     : id(id_),fio(fio_),group(group_),lessonName(LessonName_){
        
    // }
    LessonAdapter() = default;

    bool operator==(const LessonAdapter& adapter) const
    {
        return this->id == adapter.id && this->fio == adapter.fio
         && this->group == adapter.group && this->lessonName == adapter.lessonName
         && this->daysInfo == adapter.daysInfo;
    }

    bool operator!=(const LessonAdapter& adapter) const
    {
        return !(*this == adapter);
    }


    bool operator<(const LessonAdapter& adapter)
    {
        return this->id < adapter.id;
    }

    LessonAdapter& operator=(const LessonAdapter& l) = default;

    virtual void loadFromTableRow(const QTableWidget* table,int row)
    {
        Q_UNUSED(table);
        Q_UNUSED(row);

    }

    QStringList getHeaders(const QTableWidget* table);


    bool isNullptrTable(const QTableWidget* table)
    {
        if(table) return true;
        else return false;
    }

    virtual TYPE_ADAPTER getType() const { return TYPE_ADAPTER::Lesson;}

    virtual std::unique_ptr<LessonAdapter> clone() const {
        return std::make_unique<LessonAdapter>(*this);
    }

    virtual QJsonObject toJson() const
    {
        QJsonObject currentObject;
        QJsonArray datesArray;

        currentObject["ID"] = id;
        currentObject["fullName"] = fio;
        currentObject["NameLesson"] = lessonName;
        currentObject["Pair"] = columnCount > DEFAULT_TABLES ? columnCount : 0 ;
        currentObject["Group"] = group;

        for (auto it = daysInfo.begin(); it != daysInfo.end(); ++it) {
            QJsonObject dateObject;
            dateObject["Date"] = it.key();
            dateObject["Marks"] = it.value();
            datesArray.append(dateObject);
        }

        currentObject["Dates"] = datesArray;

        return currentObject;
    }

    virtual ~LessonAdapter() = default;
};


struct RecordAdapter : public LessonAdapter {
    QString recordBook;
    QString TeacherName;
    bool automatic;
    int gradeRecord;
    QString GradesStudents;
    QString Marks;


    bool operator==(const RecordAdapter& adapter) const
    {
        return LessonAdapter::operator==(adapter) &&
               this->recordBook == adapter.recordBook &&
               this->TeacherName == adapter.TeacherName &&
               this->automatic == adapter.automatic &&
               this->gradeRecord == adapter.gradeRecord;
    }

    bool operator!=(const RecordAdapter& adapter) const
    {
        return !(*this == adapter);
    }


    void loadFromTableRow(const QTableWidget* table,int row) override;
    QStringList getHeaders(const QTableWidget* table) ;

    TYPE_ADAPTER getType()const override { return TYPE_ADAPTER::Record;}

    std::unique_ptr<LessonAdapter> clone() const override {
        return std::make_unique<RecordAdapter>(*this);
    }

    QJsonObject toJson() const override
    {
        QJsonObject currentObject;

        currentObject["ID"] = id;
        currentObject["fullName"] = fio;
        currentObject["NameLesson"] = lessonName;
        currentObject["Group"] = group;
        currentObject["RecordBook"] = recordBook;
        currentObject["TeacherName"] = TeacherName;
        currentObject["Automatic"] = automatic;
        currentObject["GradeRecord"] = gradeRecord;
        currentObject["GradesStudent"] = GradesStudents;
        currentObject["MarksStudent"] = Marks;

        return currentObject;

    }

};


struct ProjectAdapter : public LessonAdapter  {
    QString recordBook;
    int grade;
    QString curator;
    QString Theme;
    QString stage;
    QDate date;
    QString extraData;

    void loadFromTableRow(const QTableWidget* table,int row) override;
    QStringList getHeaders(const QTableWidget* table) ;

    virtual std::unique_ptr<LessonAdapter> clone() const {
        return std::make_unique<ProjectAdapter>(*this);
    }

    TYPE_ADAPTER getType()const override { return TYPE_ADAPTER::Project;}

    QJsonObject toJson() const override
    {

        QJsonObject currentObject;

        currentObject["ID"] = id;
        currentObject["fullName"] = fio;
        currentObject["Group"] = group;
        currentObject["RecordBook"] = recordBook;
        currentObject["Grade"] = grade;
        currentObject["Curator"] = curator;
        currentObject["Theme"] = Theme;
        currentObject["Stage"] = stage;
        currentObject["Date"] = date.toString("dd.MM.yyyy");
        currentObject["ExtraData"] = extraData;

        return currentObject;

    }

};


struct CourseAdapter : public ProjectAdapter {
    void loadFromTableRow(const QTableWidget* table,int row) override;
    QStringList getHeaders(const QTableWidget* table) ;

    virtual std::unique_ptr<LessonAdapter> clone() const {
        return std::make_unique<CourseAdapter>(*this);
    }

    TYPE_ADAPTER getType()const override { return TYPE_ADAPTER::Course;}


    QJsonObject toJson() const override
    {

        return ProjectAdapter::toJson();

    }

};


struct DiplomaAdapter : public ProjectAdapter{

    void loadFromTableRow(const QTableWidget* table,int row) override;
    QStringList getHeaders(const QTableWidget* table) ;

    virtual std::unique_ptr<LessonAdapter> clone() const {
        return std::make_unique<DiplomaAdapter>(*this);
    }

    TYPE_ADAPTER getType()const override { return TYPE_ADAPTER::Diploma;}

    QJsonObject toJson() const override
    {

        return ProjectAdapter::toJson();

    }

};


#endif // ADAPTERTABLES_H
