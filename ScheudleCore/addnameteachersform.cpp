#include "addnameteachersform.h"
#include "ui_addnameteachersform.h"
#include"../FileManagerCore/managerfs.h"
#include<QVariant>
#include<QColor>

AddNameTeachersForm::AddNameTeachersForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddNameTeachersForm)
    , schedule(std::make_unique<DataSchedule>())
    , serviceScheudle(std::make_unique<ScheduleDataService>())
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayout);

    ManagerWorker w;
    w.CreateDir(".","Расписание");

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pushButtonSave,&QPushButton::clicked,this,[this](){
       setNamesTeachersData(ui->lineEditName->text(),ui->lineEditMiddleName->text(),ui->lineEditLastName->text());

    });


    connect(ui->pushButtonDeleteRow,&QPushButton::clicked,this,[this](){
        if (ui->tableWidget->rowCount() == 0) return;

        int lastRow = ui->tableWidget->rowCount() - 1 ;
        qDebug() << lastRow << "row";

        auto* name = ui->tableWidget->item(lastRow,0);
        auto* m = ui->tableWidget->item(lastRow,1);
        auto* l = ui->tableWidget->item(lastRow,2);


        Teacher currentTeacher{name->text(), m->text(), l->text()};

        schedule->Teachers.remove(currentTeacher);

        ui->tableWidget->removeRow(lastRow);


    });

    connect(ui->pushButtonDeleteCurrRow,&QPushButton::clicked,this,[this](){


        int curr = ui->tableWidget->currentRow();
        if (curr < 0) return;

        auto* name = ui->tableWidget->item(curr,0);
        auto* middleName = ui->tableWidget->item(curr,2);
        auto* lastName = ui->tableWidget->item(curr,1);

        Teacher currentTeacher{name->text(), lastName->text(), middleName->text()};

        schedule->Teachers.remove(currentTeacher);

        --counter;
        ui->tableWidget->removeRow(curr);

    });


    connect(ui->pushButtonGo,&QPushButton::clicked,this,[this](){
        QString file = "Расписание/teachers.json";
        if(ui->comboBoxParam->currentIndex() != 0){
            int sort = ui->comboBoxParam->currentIndex();
            qDebug() << "sort id" << sort;
            ScheduleDataService::SortTypeTeachers typeSort = static_cast<ScheduleDataService::SortTypeTeachers>(sort);
            QList<Teacher> listTeacher = schedule->Teachers.values();
            serviceScheudle->sortTeacherWithParam(listTeacher,typeSort);

            bool success = jsonParser->setDataTeachers(file,listTeacher);
            if(success) return;
        }

        bool success = jsonParser->setDataTeachers(file,schedule->Teachers);
        success ? QMessageBox::information(this,"Отправка","Успешно отправлено")
                : QMessageBox::information(this,"Отправка","Ошибка отправки");


    });

    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, [this](int row, int col) {

        qDebug() << "Клик по строке:" << row;

        auto* item0 = ui->tableWidget->item(row, 0);
        auto* item1 = ui->tableWidget->item(row, 1);
        auto* item2 = ui->tableWidget->item(row, 2);

        if (!item0 || !item1 || !item2) {
            return;
        }

        QString n = item0->text();
        QString f = item1->text();
        QString o = item2->text();

        currentClickedTeacher.create(n, f, o);

        qDebug() << "Данные собраны:" << currentClickedTeacher.getFullName();
    });

    connect(ui->tableWidget,&QTableWidget::itemChanged,this,[this](QTableWidgetItem* item){
        if (currentClickedTeacher.isEmpty()) {
            return;
        }

        schedule->Teachers.remove(currentClickedTeacher);
        int row = item->row();

        auto* nameIt = ui->tableWidget->item(row, 0);
        auto* surIt  = ui->tableWidget->item(row, 2);
        auto* midIt  = ui->tableWidget->item(row, 1);

        if (!nameIt || !surIt || !midIt) return;

        Teacher updated;
        updated.create(nameIt->text(), surIt->text(), midIt->text());

        schedule->Teachers.insert(updated);

    });

}

AddNameTeachersForm::~AddNameTeachersForm()
{
    delete ui;
}

void AddNameTeachersForm::setNamesTeachersData(const QString &name, const QString &MiddleName, const QString &LastName)
{
    int currentRow = ui->tableWidget->rowCount();
    if(name.isEmpty() || MiddleName.isEmpty() || LastName.isEmpty()) return;

    ui->lineEditName->setText("");
    ui->lineEditMiddleName->setText("");
    ui->lineEditLastName->setText("");

    schedule->Teachers.insert({name,MiddleName,LastName});

    QTableWidgetItem* NameItem = new QTableWidgetItem(name);
    QTableWidgetItem* MiddleItem = new QTableWidgetItem(MiddleName);
    QTableWidgetItem* LastItem = new QTableWidgetItem(LastName);

    ui->tableWidget->insertRow(currentRow);
    ui->tableWidget->setItem(currentRow,0,NameItem);
    ui->tableWidget->setItem(currentRow,1,MiddleItem);
    ui->tableWidget->setItem(currentRow,2,LastItem);


    ++currentRow;
}

void AddNameTeachersForm::initComboBox()
{
    ui->comboBoxParam->clear();

    ui->comboBoxParam->addItem("По умолчанию",           static_cast<int>(ScheduleDataService::SortTypeTeachers::Default));
    ui->comboBoxParam->addItem("По длине имени",         static_cast<int>(ScheduleDataService::SortTypeTeachers::Length));
    ui->comboBoxParam->addItem("По алфавиту",            static_cast<int>(ScheduleDataService::SortTypeTeachers::Lexicographical));
    ui->comboBoxParam->addItem("По имени",               static_cast<int>(ScheduleDataService::SortTypeTeachers::FirstName));
    ui->comboBoxParam->addItem("По фамилии",             static_cast<int>(ScheduleDataService::SortTypeTeachers::LastName));
    ui->comboBoxParam->addItem("По отчеству",            static_cast<int>(ScheduleDataService::SortTypeTeachers::MiddleName));
}
