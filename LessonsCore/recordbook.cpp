#include "recordbook.h"
#include "ui_recordbook.h"
#include"../mainwindow.h"
#include"../IOCore/IODataHandler.h"
#include<QItemDelegate>
#include"../confrimdialog.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QByteArray>

RecordBook::RecordBook(MainWindow* main, ITablesManager *parent)
    : ITablesManager(parent)
    , backWindow(main)
    , ser(std::make_unique<SerelizerJsonManagerLessonModule<RecordAdapter>>())
    , ui(new Ui::RecordBook)
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayout_2);
    setAttribute(Qt::WA_DeleteOnClose);
    setupConnections();

    setWindowIcon(QIcon("C:/Users/Gera/Desktop/JSONS/TestBook.png"));
    setWindowTitle("Зачеты/Рубеж");

   // serilizer->DataSerelizationMenuStudentRecords(ui->tableWidget,this,ModeSerelization::Records);

    QTimer* timer = new QTimer(this);

    connect(timer,&QTimer::timeout,this,[this](){
        ser->run(ui->tableWidget);
    });

    timer->start(10'000);
    ui->lineEditFind->setPlaceholderText("Поиск по имени...");

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(150);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

}

RecordBook::~RecordBook()
{
    delete ui;
    delete obj;
    delete ihs;
}

void RecordBook::Back()
{
    backWindow->show();
    this->close();
}


void RecordBook::AutoTest(QTableWidgetItem *checkItem)
{


    if (checkItem->column() != checkboxColumn) return;
    if (checkItem->checkState() != Qt::Checked) return;

    qDebug() << "ячейка " << checkItem->row() << checkItem->column();

    QString gradesStr = ui->tableWidget->item(checkItem->row(), 6)->text();
    QList<int> gradeNumbers;

    for(const QChar& c : gradesStr) {
        if(c.isDigit()){
            gradeNumbers.append(c.digitValue());
        }
    }

    auto avgOpt = ihs->handleAutoTestStudent(checkItem, gradeNumbers);

    if (avgOpt) {
        ui->tableWidget->setItem(checkItem->row(), resultColumn,new QTableWidgetItem(QString::number(*avgOpt)));
    }
}



void RecordBook::FindName()
{
    ui->tableWidget->clearSelection();
    QString name = ui->lineEditFind->text();
    bool found = false;

    for(int row = 0; row<ui->tableWidget->rowCount();++row)
    {
        for(int col = 0; col<ui->tableWidget->columnCount();++col)
        {
            QTableWidgetItem* item = ui->tableWidget->item(row,col);

            if(item)
            {

                if(item->text().contains(name,Qt::CaseInsensitive))
                {
                        found = true;
                        ui->tableWidget->setCurrentItem(item);
                        break;
                }

            }
        }
        if(found) break;
    }

    if(!found)
    {
         QMessageBox::warning(this,"Поиск", "Нечего не найдено");
    }

    return;
}

void RecordBook::DeleteRow()
{

    ConfrimDialog dlg(this);

    if(ui->checkBoxConfrmDelete->checkState())
    {
        if(dlg.exec() == QDialog::Accepted)
        {
            int select = ui->tableWidget->currentRow();
            if(select>= 0)
            {
                ui->tableWidget->removeRow(select);
            }
        }
        else
        {
            return;
        }
    } else
    {
        int select = ui->tableWidget->currentRow();
        if(select>= 0)
        {
            ui->tableWidget->removeRow(select);
        }
    }
}

void RecordBook::ClearTable()
{

    ConfrimDialog dlg(this);

    if(ui->checkBoxConfrmDelete->checkState())
    {
        if(dlg.exec() == QDialog::Accepted)
        {
            ui->tableWidget->setRowCount(0);
        }
        else
        {
            return;
        }
    }
    else
    {

        ui->tableWidget->setRowCount(0);
    }
}

void RecordBook::addRow()
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    for(int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        if(i == checkboxColumn)
        {
            QTableWidgetItem *checkItem = new QTableWidgetItem();
            checkItem->setFlags(checkItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            checkItem->setCheckState(Qt::Unchecked);
            ui->tableWidget->setItem(row, i, checkItem);

        }
        else{
            ui->tableWidget->setItem(row, i, new QTableWidgetItem(""));
        }
    }
}

void RecordBook::PrintData()
{
    ITablesManager::PrinterData();
}

QString RecordBook::SetHeaderDocument()
{
    return "<tr><th>ID</th><th>ФИО</th><th>Группа</th><th>Зачетная книжка</th><th>Преподователь</th><th>Название предмета</th><th>Оценки Студента</th><th>Марки Студента<\th><th>Автомат</th><th>Оценка за зачет</th></tr>";

}


QString RecordBook::SetNameDocument()
{
    return "<h2 align='center'>Отчёт по успеваимости</h2>";
}

QString RecordBook::setFormat(const QJsonObject &obj)
{

    QString id = obj["ID"].toString();
    QString fio = obj["ФИО"].toString();
    QString group = obj["Группа"].toString();
    QString testBook = obj["Зачетная книжка"].toString();
    QString nameLesson = obj["Название предмета"].toString();
    QString gradeTest = obj["Оценка за зачет"].toString();
    QJsonArray grades = obj["Оценки Студента"].toArray();
    QJsonArray marks = obj["Марки Студента"].toArray();
    QString autoTest = obj["Автомат"].toString();
    QString TeacherName=obj["Преподователь"].toString();

    QStringList grade;
    QStringList mark;
    for(const auto& c: grades)
    {
        grade << c.toString();
    }
    for(const auto& c: marks)
    {
        mark << c.toString();
    }




   QString html = QString("<tr>"
                    "<td>%1</td>"
                    "<td>%2</td>"
                    "<td>%3</td>"
                    "<td>%4</td>"
                    "<td>%5</td>"
                    "<td>%6</td>"
                    "<td>%7</td>"
                    "<td>%8</td>"
                    "<td>%9</td>"
                    "<td>%10</td>"
                    "</tr>")
                .arg(id, fio, group,testBook,TeacherName,nameLesson,grade.join(""),mark.join(""),autoTest, gradeTest);

    return html;

}


void RecordBook::setupConnections()
{
    obj = new ImportSaveData();
    ihs = new LogicOperation();
    fileManager  = new FileManager(this);
    serilizer = std::make_unique<SerializerData>();
    dataWriterInfo = std::make_unique<DataWritterStudent>();


    connect(ui->lineEditFind,&QLineEdit::textChanged,this,&RecordBook::FindName);
    connect(ui->btnAdd,&QPushButton::clicked,this,&RecordBook::addRow);
    connect(ui->btnDelete,&QPushButton::clicked,this,&RecordBook::DeleteRow);
    connect(ui->tableWidget,&QTableWidget::itemChanged,this,&RecordBook::AutoTest);


    QMenuBar *menuBar = new QMenuBar(this);

    QMenu *Function = new QMenu("Функционал", menuBar);
    SaveData = new QAction("Сохранить в Json", menuBar);
    uploadReadyData = new QAction("Выгрузить из JSON", menuBar);
    Print = new QAction("Печатать в PDF", menuBar);
    SaveRecordJSON = new QAction("Вывести рубеж JSON", menuBar);
    SaveRecordPDF = new QAction("Вывести рубеж PDF",menuBar);

    Clear = new QAction("Очистить таблицу",menuBar);
    Quit = new QAction("Выйти", menuBar);

    Function->addAction(SaveData);
    Function->addAction(uploadReadyData);
    Function->addAction(Print);
    Function->addAction(Quit);
    Function->addAction(SaveRecordJSON);
    Function->addAction(Clear);
    Function->addAction(SaveRecordPDF);

    menuBar->addMenu(Function);


    ui->gridLayout_2->addWidget(menuBar);

    connect(SaveData, &QAction::triggered, this, [this](){
        obj->saveToJSonRecordBook(ui->tableWidget, this);
    });

    connect(uploadReadyData, &QAction::triggered, this, [this](){
        obj->loadFromRecordBook(ui->tableWidget, this);
    });

    connect(Quit, &QAction::triggered, this, &RecordBook::Back);
    connect(Clear, &QAction::triggered, this, &RecordBook::ClearTable);
    connect(Print, &QAction::triggered, this, &RecordBook::PrintData);

    connect(SaveRecordPDF,&QAction::triggered,this,[this](){
        //FileManager m;
        dataWriterInfo->writeSimplePDF("df");
    });
    connect(SaveRecordJSON,&QAction::triggered,this,[this](){

        LogicOperation o;
        for(int row = 0; row < ui->tableWidget->rowCount();row++)
        {
            StudentStructSimple student;

            student.ID = ui->tableWidget->item(row,0)->text();
            student.FullName = ui->tableWidget->item(row,1)->text();
            student.Group = ui->tableWidget->item(row,2)->text();
            student.recordBook = ui->tableWidget->item(row,3)->text();
            student.TeacherName = ui->tableWidget->item(row,4)->text();
            student.LessonName = ui->tableWidget->item(row,5)->text();
            student.averageBall = ui->tableWidget->item(row,9)->text().toDouble();
            student.autoTest = ui->tableWidget->item(row,8)->checkState() ? "Да" : "Нет";
            student.countPair = (o.GetValueRegEditKeys("columnCount","Lessons").toInt()) - 4;

            for(const QChar& c : ui->tableWidget->item(row,6)->text())
                student.grades.append(c);

            for(const QChar& c : ui->tableWidget->item(row,7)->text())
                student.marks.append(c);

            dataWriterInfo->writeSimple(student);

        }
    });
}
