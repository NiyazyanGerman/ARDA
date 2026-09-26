#ifndef PRIMARYDATASCHEUDLEMANAGERFORM_H
#define PRIMARYDATASCHEUDLEMANAGERFORM_H

#include <QWidget>
#include<QPushButton>
#include<QStackedWidget>
#include"addcabinetsdialog.h"
#include"addlessontimeform.h"
#include"addnameteachersform.h"
#include"addlessonnameform.h"
class Add_Lesson_Schedule_Dialog;

namespace Ui {
class PrimaryDataScheudleManagerForm;
}

class PrimaryDataScheudleManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit PrimaryDataScheudleManagerForm(Add_Lesson_Schedule_Dialog* ALSD, QWidget *parent = nullptr);
    ~PrimaryDataScheudleManagerForm();

private slots:
    void onTeachersClicked();
    void onCabinetsClicked();
    void onLessonTimeClicked();
    void onLessonNameClicked();

private:
    void backMenu();
    void setupForm();
    void highlightButton(QPushButton *activeBtn);

    Ui::PrimaryDataScheudleManagerForm *ui;

    QPushButton* btnAddLesson;
    QPushButton* btnAddCabinets;
    QPushButton* btnAddTeacher;

    AddCabinetsDialog* cabinetsPage;
    AddLessonTimeForm* lessonPage;
    AddNameTeachersForm* teachesPage;
    AddLessonNameForm* lessonName;

    Add_Lesson_Schedule_Dialog* MainMenu;

};

#endif // PRIMARYDATASCHEUDLEMANAGERFORM_H
