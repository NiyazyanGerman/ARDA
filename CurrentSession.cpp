#include "CurrentSession.h"
#include "ActiveSession.h"
#include "ui_CurrentSession.h"

CurrentSession::CurrentSession(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CurrentSession)
{
    ui->setupUi(this);
}

CurrentSession::~CurrentSession()
{
    delete ui;
}

void CurrentSession::initUI(const ActiveSession *session)
{
    ui->lblFullName->setText(ui->lblFullName->text() + " " + QString::fromStdString(session->getFullName()));
    ui->lblEmail->setText(ui->lblEmail->text() + " " + QString::fromStdString(session->getEmail()));
    ui->lblSessionStart->setText(ui->lblSessionStart->text() + " " + session->getLoginTime().toString("hh.MM.ss"));
    ui->lblSessionExpires->setText(ui->lblSessionExpires->text() + " " + session->getExpiresTime().toString("hh.MM.ss"));
    return;
}
