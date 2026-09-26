#include "AddHintsForm.h"
#include "ui_AddHintsForm.h"
#include<QLineEdit>
AddHintsForm::AddHintsForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddHintsForm)
    , serviceScheudle(std::make_unique<ScheduleDataService>())
    , scheudle(std::make_unique<DataSchedule>())
    , jsonParser(std::make_unique<ScheduleJsonParser>())
{
    ui->setupUi(this);
    initComboBox();
    connect(ui->lineEditSeacrhTeacher, &QLineEdit::textChanged, this, [this](const QString& teacher){
        int index = ui->comboBoxTeacher->findText(teacher, Qt::MatchContains | Qt::MatchContains);
        if (index != -1) {
            ui->comboBoxTeacher->setCurrentIndex(index);
        }
    });
    connect(ui->lineEditSearchLesson, &QLineEdit::textChanged, this, [this](const QString& lessName){
        int index = ui->comboBoxLessonName->findText(lessName, Qt::MatchContains | Qt::MatchContains);
        if (index != -1) {
            ui->comboBoxLessonName->setCurrentIndex(index);
        }
    });

    connect(ui->btnSave,&QPushButton::clicked,this,[this](){
        QStringList parts = ui->comboBoxTeacher->currentText().split(' ');
        Teacher currTeacher{parts[0],parts[1],parts[2]};
        QString currLesson = ui->comboBoxLessonName->currentText();
        scheudle->hintsTeachers[currTeacher].append(currLesson);

        ui->listWidget->addItem(new QListWidgetItem(ui->comboBoxTeacher->currentText() + "--" + currLesson));

    });

    connect(ui->btnGo,&QPushButton::clicked,this,[this](){
        jsonParser->setHintsTeachesrLessonName("Hints.json",scheudle->hintsTeachers);
    });
}

AddHintsForm::~AddHintsForm()
{
    delete ui;
}

void AddHintsForm::initComboBox()
{
    ui->comboBoxLessonName->addItems({serviceScheudle->GetLessonNameJson().values()});

    for(const auto& teacher : serviceScheudle->GetTeacherNameJson())
    {
        ui->comboBoxTeacher->addItem(teacher.getFullName());
    }

}
