#include "SetColumnTableDilog.h"
#include "ui_SetColumnTableDilog.h"

SetColumnTableDilog::SetColumnTableDilog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SetColumnTableDilog)
{
    ui->setupUi(this);
}

SetColumnTableDilog::~SetColumnTableDilog()
{
    delete ui;
}
