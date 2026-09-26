#include "primarydatascheudlemanagerform.h"
#include "ui_primarydatascheudlemanagerform.h"
#include<QPropertyAnimation>
#include<QParallelAnimationGroup>
#include"add_lesson_schedule_dialog.h"

PrimaryDataScheudleManagerForm::PrimaryDataScheudleManagerForm(Add_Lesson_Schedule_Dialog *ALSD, QWidget *parent)
    : QWidget(parent)
    , MainMenu(ALSD)
    , ui(new Ui::PrimaryDataScheudleManagerForm)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    setupForm();

    setWindowTitle("Выставка первоначальных данных");
    this->setLayout(ui->gridLayout);

    connect(ui->btnTeacher, &QPushButton::clicked, this, &PrimaryDataScheudleManagerForm::onTeachersClicked);
    connect(ui->btnCab, &QPushButton::clicked, this, &PrimaryDataScheudleManagerForm::onCabinetsClicked);
    connect(ui->btnLesson, &QPushButton::clicked, this, &PrimaryDataScheudleManagerForm::onLessonTimeClicked);
    connect(ui->btnLessonName,&QPushButton::clicked,this,&PrimaryDataScheudleManagerForm::onLessonNameClicked);
    connect(ui->btnQuit,&QPushButton::clicked,this,&PrimaryDataScheudleManagerForm::backMenu);

    onLessonTimeClicked();
}

PrimaryDataScheudleManagerForm::~PrimaryDataScheudleManagerForm()
{
    delete ui;
}

void PrimaryDataScheudleManagerForm::onTeachersClicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    highlightButton(ui->btnTeacher);
}

void PrimaryDataScheudleManagerForm::onCabinetsClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    highlightButton(ui->btnCab);
}

void PrimaryDataScheudleManagerForm::onLessonTimeClicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    highlightButton(ui->btnLesson);
}

void PrimaryDataScheudleManagerForm::onLessonNameClicked()
{

    ui->stackedWidget->setCurrentIndex(3);

    highlightButton(ui->btnLessonName);
}

void PrimaryDataScheudleManagerForm::backMenu()
{
    this->close();
    MainMenu->show();

}


void PrimaryDataScheudleManagerForm::setupForm()
{
    lessonPage = new AddLessonTimeForm(this);
    cabinetsPage = new AddCabinetsDialog(this);
    teachesPage = new AddNameTeachersForm(this);
    lessonName = new AddLessonNameForm(this);

    while (ui->stackedWidget->count() > 0) {
        QWidget* widget = ui->stackedWidget->widget(0);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }

    ui->stackedWidget->addWidget(lessonPage);
    ui->stackedWidget->addWidget(cabinetsPage);
    ui->stackedWidget->addWidget(teachesPage);
    ui->stackedWidget->addWidget(lessonName);


}


void PrimaryDataScheudleManagerForm::highlightButton(QPushButton *activeBtn)
{
    QList<QPushButton*> navButtons = {
        ui->btnLesson,
        ui->btnCab,
        ui->btnTeacher,
        ui->btnLessonName,

    };

    for (QPushButton *btn : navButtons) {
        if (btn == activeBtn) {
            btn->setStyleSheet(
                "QPushButton {"
                "    background-color: #0078d7;"
                "    color: white;"
                "    border: 2px solid #005a9e;"
                "    font-weight: bold;"
                "    padding: 8px;"
                "}"
                );
        } else {
            btn->setStyleSheet(
                "QPushButton {"
                "    background-color: #f0f0f0;"
                "    color: #333333;"
                "    border: 1px solid #cccccc;"
                "    padding: 8px;"
                "}"
                "QPushButton:hover {"
                "    background-color: #e0e0e0;"
                "}"
                );
        }
    }
}

