#include"AdapterTables.h"


void RecordAdapter::loadFromTableRow(const QTableWidget *table, int row)
{

    for(int col = 0; col < table->columnCount(); col++)
    {
        QTableWidgetItem* headerItem = table->horizontalHeaderItem(col);
        if (!headerItem) continue;

        QString header = headerItem->text();
        QTableWidgetItem* item = table->item(row, col);

        if (!item) continue;

        if (header == "Зачетная книжка") recordBook = item->text();
        else if (header == "Преподователь") TeacherName = item->text();
        else if(header == "Название Предмета") lessonName = item->text();
        else if (header == "Автомат") automatic = (item->checkState() == Qt::Checked);
        else if (header == "Оценка за зачет") gradeRecord = item->text().toInt();
        else if (header == "Оценки Студента") GradesStudents = item->text();
        else if (header == "Марки Студента") Marks = item->text().toLower();

    }
}
void ProjectAdapter::loadFromTableRow(const QTableWidget *table, int row)
{

    for(int col = 0; col < table->columnCount(); col++)
    {
        QString header = table->horizontalHeaderItem(col)->text();
        QTableWidgetItem* item = table->item(row, col);

        if (!item) continue;

        if (header == "Зачетная книжка") recordBook = item->text();
        else if (header == "Тема") Theme = item->text();
        else if (header == "Куратор/Руководитель") curator = item->text();
        else if (header == "Этап Работы") stage = item->text();
        else if (header == "Оценка") grade = item->text().toInt();
        else if (header == "Дата Сдачи") date = QDate::fromString(item->text(), "dd.MM.yyyy");
        else if (header == "Доп. Данные") extraData = item->text();


    }
}
QStringList LessonAdapter::getHeaders(const QTableWidget *table)
{
    QStringList headers;

    auto isNull =  isNullptrTable(table);
    if(!isNull) return {};

    for(int col = 0 ; col <table->columnCount(); col++)
    {
        QTableWidgetItem* headerItem = table->horizontalHeaderItem(col);
        if (headerItem) {
            headers << headerItem->text();
        }
    }

    return headers;
}


QStringList RecordAdapter::getHeaders(const QTableWidget *table)
{
   return LessonAdapter::getHeaders(table);
}


QStringList ProjectAdapter::getHeaders(const QTableWidget *table)
{

    return LessonAdapter::getHeaders(table);
}

void CourseAdapter::loadFromTableRow(const QTableWidget *table, int row)
{

    ProjectAdapter::loadFromTableRow(table,row);
}

QStringList CourseAdapter::getHeaders(const QTableWidget *table)
{

    return LessonAdapter::getHeaders(table);
}

void DiplomaAdapter::loadFromTableRow(const QTableWidget *table, int row)
{

    ProjectAdapter::loadFromTableRow(table,row);
}

QStringList DiplomaAdapter::getHeaders(const QTableWidget *table)
{

    return LessonAdapter::getHeaders(table);
}


