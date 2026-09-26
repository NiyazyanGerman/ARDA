#include "ITablesManager.h"
#include"../Validator.h"
#include<QPrinter>
#include<QPrintDialog>
#include<QPainter>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QByteArray>
#include<QTextDocument>

ITablesManager::ITablesManager(QWidget *parent)
    : QWidget{parent}
{

}

void ITablesManager::PrinterData()
{

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    QString filePDF = fileChoicer->saveFilePDF();
    printer.setOutputFileName(filePDF);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QFile file(fileChoicer->chooseFileJson());

    try
    {
        Validator::isFileValid(file,ModeValidator::ReadFile);
    }
    catch(const std::logic_error& e)
    {
        QMessageBox::warning(this,"Error",e.what());
    }

    QByteArray byteArr = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(byteArr);
    QJsonArray arr = doc.array();

    QString html = QString("<h2 align='center'>%1</h2>").arg(SetNameDocument());
    html += "<table border='1' cellspacing='0' cellpadding='4' width='100%'>";
    html += SetHeaderDocument();

    for(const auto& c : arr)
    {
        if(!c.isObject()) continue;

        QJsonObject obj = c.toObject();
        html += setFormat(obj);

    }


    html += "</table>";

    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);

    QMessageBox::information(this, "Успешно", "Файл " + filePDF + " Создан!" );


}

void ITablesManager::init()
{
    fileChoicer = std::make_unique<FileManager>();
}
