#include "AbiturientForm.h"
#include "ui_AbiturientForm.h"

AbiturientForm::AbiturientForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AbiturientForm)
{
    ui->setupUi(this);
}

AbiturientForm::~AbiturientForm()
{
    delete ui;
}
