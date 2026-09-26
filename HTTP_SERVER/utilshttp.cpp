#include "utilshttp.h"
#include<QDebug>
#include<windows.h>
#include<QFile>


UtilsHTTP::UtilsHTTP(QObject *parent)
    : QObject{parent}
{}

QString UtilsHTTP::backDirServerConstructPath(const QString &curentPath)
{

    qDebug() << "first path" << curentPath;
    auto endPoint = curentPath.lastIndexOf("/");

    curentPath =  curentPath.left(endPoint);
    auto TwoPoint = curentPath.lastIndexOf("/");

    curentPath =  curentPath.left(TwoPoint);


    qDebug() << "two path" << curentPath;
    return curentPath;

}

QString UtilsHTTP::getRegisterLogin()
{

}

