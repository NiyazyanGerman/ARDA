/*
 * Project: ARDA Student Manager
 * Author: German Niyazyan (Gerakl1123)
 * License: CC BY-NC 4.0 — Non-commercial use only
 *
 * © 2025 German Niyazyan
 * https://github.com/Gerakl1123/ARDA_Stud_Manager
 * https://creativecommons.org/licenses/by-nc/4.0/
 */

#ifndef AUTHREGWINDOW_H
#define AUTHREGWINDOW_H

#include "ActiveSession.h"
#include"AuthUser.h"
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include"CurrentSession.h"
class AuthRegWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AuthRegWindow(QWidget *parent = nullptr);


    ~AuthRegWindow();

private slots:
    void onAuthClicked();
    void onRegClicked();

    void onAuthSucces();
private:
    inline static UserAuthenticator Authenticator{"Auth.log"};

    QLineEdit*   lineEditLogin;
    QLineEdit*   lineEditPassword;
    QPushButton* pushButtonAuth;
//    QPushButton* pushButtonReg;
    QByteArray responseJson;
    QNetworkAccessManager* manager;
    ActiveSession* session;
signals:
     void authSuccess();
};

#endif // AUTHREGWINDOW_H
