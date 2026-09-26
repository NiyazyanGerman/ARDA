#include "clientwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QUrlQuery>
#include <QFile>
#include <QFileDialog>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

ClientWidget::ClientWidget(QWidget *parent)
    : QWidget(parent)
{

    QLabel *label = new QLabel("Путь к файлу на сервере:", this);
    lineEditPath = new QLineEdit(this);
    buttonDownload = new QPushButton("Скачать", this);
    buttonGetList = new QPushButton("df",this);
    textEditLog = new QTextEdit(this);
    textEditLog->setReadOnly(true);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(label);
    topLayout->addWidget(lineEditPath);
    topLayout->addWidget(buttonDownload);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(textEditLog);

    setLayout(mainLayout);
    setWindowTitle("HTTPS Client");

    manager = new QNetworkAccessManager(this);
    // connect(manager, &QNetworkAccessManager::finished,
    //         this, &ClientWidget::onFinished);

    connect(manager, &QNetworkAccessManager::finished,
            this, &ClientWidget::onFinishedGetList);

    // connect(buttonDownload, &QPushButton::clicked,
    //         this, &ClientWidget::onDownloadClicked);

    connect(buttonGetList, &QPushButton::clicked,
            this, &ClientWidget::onGetList);



    resize(600, 200);
}

void ClientWidget::onDownloadClicked()
{
    QString path = lineEditPath->text().trimmed();
    if (path.isEmpty()) {
        textEditLog->append("Введите путь файла для скачивания");
        return;
    }

    QUrl url("https://192.168.0.184:8443/api/DownloadFile");
    QUrlQuery query;
    query.addQueryItem("path", path);
    url.setQuery(query);

    textEditLog->append("Отправляю запрос: " + url.toString());

    QNetworkRequest request(url);
    manager->get(request);
}


void ClientWidget::onFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        textEditLog->append("Ошибка сети: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    QString disposition = reply->rawHeader("Content-Disposition");
    QString filename = "downloaded_file";

    if (!disposition.isEmpty()) {
        QRegularExpression rx("filename=\"([^\"]+)\"");
        QRegularExpressionMatch match = rx.match(disposition);
        if (match.hasMatch()) {
            filename = match.captured(1);
        }
    }

    QString savePath = QFileDialog::getSaveFileName(this,
                                                    "Сохранить файл как", filename);

    if (savePath.isEmpty()) {
        textEditLog->append("Сохранение отменено");
        reply->deleteLater();
        return;
    }

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly)) {
        textEditLog->append("Не удалось открыть файл для записи");
        reply->deleteLater();
        return;
    }

    file.write(reply->readAll());
    file.close();

    textEditLog->append("Файл сохранён: " + savePath);

    reply->deleteLater();
}

void ClientWidget::onGetList()
{
    QString path = lineEditPath->text().trimmed();
    if (path.isEmpty()) {
        textEditLog->append("Введите путь файла для скачивания");

    }

    QUrl url("https://192.168.0.184:8443/api/ListFile");
    QUrlQuery query;
    query.addQueryItem("path", path);
    url.setQuery(query);

    textEditLog->append("Отправляю запрос: " + url.toString());

    QNetworkRequest request(url);
    manager->get(request);

}

void ClientWidget::onFinishedGetList(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        textEditLog->append("Ошибка сети: " + reply->errorString());
        reply->deleteLater();
        return;
    }



    QFile file("Struct.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        textEditLog->append("Не удалось открыть файл");
        return;
    }
    file.write(reply->readAll());
    file.close();

    textEditLog->append("Файл сохранён: " + file.fileName());

    reply->deleteLater();
}


