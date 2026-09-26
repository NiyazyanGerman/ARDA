#include "mainwindow.h"
#include"authregwindow.h"
#include <QApplication>
#include<QSslCertificate>
#include<QSslConfiguration>
#include"FileManagerCore/managerfs.h"
#include"LessonsCore/texpanel.h"
#include<sodium.h>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    TexPanel p;
    QString certPath = "server.crt";
    QFile certFile(certPath);
    if(certFile.open(QIODevice::ReadOnly)) {
        QSslCertificate cert(&certFile, QSsl::Pem);
        QSslConfiguration config = QSslConfiguration::defaultConfiguration();
        config.addCaCertificate(cert);
        QSslConfiguration::setDefaultConfiguration(config);
        certFile.close();
        qDebug() << "Certificate loaded from:" << certPath;
    } else {
        qDebug() << "CRITICAL: Certificate not found at" << certPath;
    }


    if (sodium_init() < 0) {
        return -1;
    }

    AuthRegWindow winAR;
    //CurrentSession s;
    MainWindow mainWindow;
    QObject::connect(&winAR,&AuthRegWindow::authSuccess,[&mainWindow,&winAR](){
        mainWindow.show();
        winAR.close();

    });
    ManagerWorker fs;
    fs.CreateFile(QDir::currentPath(), "Works.json");
    mainWindow.show();
   //mainWindow.show();

    return a.exec();
}
