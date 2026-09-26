#include "TableStudentInfoForm.h"
#include "ui_tablestudentinfoform.h"

TableStudentInfoForm::TableStudentInfoForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TableStudentInfoForm)
{
    ui->setupUi(this);
}

TableStudentInfoForm::~TableStudentInfoForm()
{
    delete ui;
}
