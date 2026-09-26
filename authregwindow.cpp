/*
 * Project: ARDA Student Manager
 * Author: German Niyazyan (Gerakl1123)
 * License: CC BY-NC 4.0 — Non-commercial use only
 *
 * © 2025 German Niyazyan
 * https://github.com/Gerakl1123/ARDA_Stud_Manager
 * https://creativecommons.org/licenses/by-nc/4.0/
 */

#include "authregwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

#include<QSslError>
#include"ActiveSession.h"
AuthRegWindow::AuthRegWindow(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(R"(
        QWidget {
            background-color: #0c0c14;
            color: #d1d1e0;
            font-family: 'Segoe UI', sans-serif;
        }
        QLineEdit {
            background-color: #1e1e2e;
            border: 1px solid #2e2e42;
            border-radius: 12px;
            padding: 10px 15px;
            color: #ffffff;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 1px solid #3390EC;
            background-color: #252538;
        }
        QPushButton {
            background-color: #3390EC;
            color: #ffffff;
            border-radius: 12px;
            padding: 10px;
            font-weight: 600;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #4a9ff5;
        }
        QPushButton:pressed {
            background-color: #2c5588;
        }
        /* Стиль для кнопки Регистрации (более спокойный) */
        QPushButton#pushButtonReg {
            background-color: transparent;
            border: 1px solid #2e2e42;
            color: #8e8e9e;
        }
        QPushButton#pushButtonReg:hover {
            background-color: #1e1e2e;
            color: #ffffff;
        }
    )");
    setWindowIcon(QIcon("C:/Users/Gera/Desktop/JSONS/AU.png"));
    setWindowTitle("Авторизация / Регистрация");

    lineEditLogin = new QLineEdit(this);
    lineEditLogin->setPlaceholderText("Логин");

    lineEditPassword = new QLineEdit(this);
    lineEditPassword->setPlaceholderText("Пароль");
    lineEditPassword->setEchoMode(QLineEdit::Password);

    pushButtonAuth = new QPushButton("Войти",this);
   // pushButtonReg = new QPushButton("Зарегестрироваться",this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(lineEditLogin);
    layout->addWidget(lineEditPassword);
    layout->addWidget(pushButtonAuth);
    //layout->addWidget(pushButtonReg);

    manager = new QNetworkAccessManager;
    setLayout(layout);

    connect(pushButtonAuth,&QPushButton::clicked,this,&AuthRegWindow::onAuthClicked);
 //   connect(pushButtonReg,&QPushButton::clicked,this,&AuthRegWindow::onRegClicked);


    ActiveSession::instance()->generatedToken(64);

}

AuthRegWindow::~AuthRegWindow()
{

    delete manager;
}

void AuthRegWindow::onAuthClicked()
{
    QString login = lineEditLogin->text();
    QString password = lineEditPassword->text();


    QJsonObject auth;
    auth["TableId"] = 301;
    auth["login"] = login;
    auth["password"] = password;
    auth["token"] = ActiveSession::instance()->getToken();
    qDebug() <<ActiveSession::instance()->getToken();
    QByteArray data = QJsonDocument(auth).toJson();
    QNetworkRequest request(QUrl("https://127.0.0.1:1123/api/Auth"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager->post(request,data);
    reply->ignoreSslErrors();

    connect(reply, &QNetworkReply::finished, [reply,this]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit authSuccess();
            responseJson = reply->readAll();
            QMessageBox::information(this, "Авторизация", "Успешно");
            QJsonDocument doc = QJsonDocument::fromJson(responseJson);

            QJsonObject obj = doc.object();
            std::string fullname = (obj["last_name"].toString() + " " +
                                    obj["first_name"].toString() + " " +
                                    obj["middle_name"].toString()).toStdString();
                std::string departament = (obj["departament"].toString().toStdString());

            qDebug() << " " << fullname;

            ActiveSession::instance()->initSession(fullname,"3e3");

        } else {
            QMessageBox::information(this, "Авторизация", QString::fromUtf8(reply->readAll()));
        }
        reply->deleteLater();
    });

}

void AuthRegWindow::onRegClicked()
{
    QString login = lineEditLogin->text();
    QString password = lineEditPassword->text();

    bool reg = Authenticator.registerStudent(login, password);


    if (reg) {
        QMessageBox::information(this, "Регистрация", "Успешно!");
        emit authSuccess();

    } else {
        QMessageBox::warning(this, "Регистрация", "Не удалось зарегистрироваться. Или Логин уже занят");
        return;
    }
}

void AuthRegWindow::onAuthSucces()
{


    // session->generatedToken(64);

    // QJsonObject sessions;
    // sessions["TableId"] = 105;
    // sessions["token"] = *(session->getToken().get());
    // sessions["login"] = lineEditLogin->text();
    // sessions["password"] = lineEditPassword->text();

    // QByteArray data = QJsonDocument(sessions).toJson();
    // QNetworkRequest request(QUrl("https://127.0.0.1:1123/api/InsertDataTable"));
    // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // QNetworkReply *reply = manager->post(request,data);
    // reply->ignoreSslErrors();

    // connect(reply, &QNetworkReply::finished, [reply,this]() {
    //     if (reply->error() == QNetworkReply::NoError) {
    //         QString response = QString::fromUtf8(reply->readAll());
    //         qDebug() << response;




    //     } else {
    //         QMessageBox::information(this, "Авторизация", QString::fromUtf8(reply->readAll()));
    //     }
    //     reply->deleteLater();
    // });


}
