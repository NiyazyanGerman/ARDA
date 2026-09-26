#include "httpserverform.h"
#include "ui_HTTPserverForm.h"
#include<QDesktopServices>
#include<QTableWidget>
#include<QNetworkReply>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonValue>
#include<QUrlQuery>
#include"../FileChooicer.h"
#include<QDir>
#include<QFrame>
#include<QLabel>
#include<QtConcurrent/QtConcurrent>

HTTPserverForm::HTTPserverForm(MainWindow *qMain, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HTTPserverForm)
    , mainWin(qMain)
    , manager(new QNetworkAccessManager(this))
{

    ui->setupUi(this);
    init();

  connect(ui->lineEditIP,&QLineEdit::editingFinished,this,[this](){
      infoServer->setIp(ui->lineEditIP->text());

  });
  connect(ui->lineEditPort,&QLineEdit::editingFinished,this,[this](){
      infoServer->setPort(ui->lineEditPort->text().toInt());

  });

  connect(ui->treeViewServerFS,&QTreeView::clicked,[this](const QModelIndex& index){
      int row = index.row();
     QString type = index.sibling(row, 1).data(Qt::DisplayRole).toString();
      if(type == "Файл") return;

      QFile fileR("StructFS.json");

      if (!fileR.open(QIODevice::ReadOnly))
          return;

        QString name = index.data().toString();
        QString pathCurent = "";
        QByteArray bArr = fileR.readAll();

    QJsonDocument doc  = QJsonDocument::fromJson(bArr);
    QJsonArray array = doc.array();

    for (const QJsonValue &v : array)
    {
        if(!v.isObject()) continue;

        auto obj = v.toObject();
        if(obj["p"].toString().contains(name))
        {
            pathCurent = obj["p"].toString();
        }else {
            continue;
        }

    }
    connect(manager, &QNetworkAccessManager::sslErrors,[](QNetworkReply* reply, const QList<QSslError>& errors){
        for (const auto &e : errors)
            qDebug() << "SSL error:" << e.errorString();

        reply->ignoreSslErrors();
    });

    ui->labelServerCurrentPath->setText(pathCurent);

    QUrl url = QString(("https://%1:%2/api/ListFile")).arg(infoServer->ip()).arg(QString::number(infoServer->port()));
      QUrlQuery query;
      query.addQueryItem("path", pathCurent);
      url.setQuery(query);

      QNetworkRequest request(url);
      qDebug() << url;

      auto reply = manager->get(request);

      QFile *file = new QFile("StructFS.json");
      if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
          file->deleteLater();
          reply->deleteLater();
          return;
      }

      connect(reply, &QNetworkReply::readyRead, [reply, file]() {
          file->write(reply->readAll());
      });

      connect(reply, &QNetworkReply::finished, [this, reply, file]() {
          file->close();
          qDebug()<<  file->size();
          ConstructTreeView();
          reply->deleteLater();
          file->deleteLater();

      });

  });


    connect(ui->btnBackDir,&QPushButton::clicked,[this](){
      QString pathCurent = jsonManager->getFullPath();

        apiHelper->GetBackDir(utilsHelperHttp->backDirServerConstructPath(pathCurent),infoServer);

        ConstructTreeView();
  });

  connect(ui->btnUpload,&QPushButton::clicked,this,[this](){

    ui->textEditLogLocal->append("Инициализирую POST Запрос");
      FileManager choicerFile;
      QString pathUploadFile = choicerFile.chooseFile();

      // QModelIndex indexPathServer = ui->treeViewServerFS->currentIndex();
      // if(indexPathServer.column() != 0) return;

      // QString nameFile = modelTree->data(indexPathServer).toString();
      // QString tempPath = jsonManager->getFullPath(nameFile);
      // QDir dir(tempPath);
      // dir.cdUp();
      // QString fullpath = dir.absolutePath();
      qDebug() << pathUploadFile;
      auto* uploadFile = new UploadFileHTTP(infoServer);

      qDebug() << pathUploadFile;

      QProgressBar* freeBar = nullptr;

      if (ui->progressBarUploadOne->value() == 0) freeBar = ui->progressBarUploadOne;
      else if (ui->progressBarUploadTwo->value() == 0) freeBar = ui->progressBarUploadTwo;
      else if (ui->progressBarUploadThree->value() == 0) freeBar = ui->progressBarUploadThree;

      uploadInfo.insert(uploadFile,freeBar);

      connect(uploadFile,&UploadFileHTTP::UploadProgress,this,[this,uploadFile](qint64 r,qint64 t){
          auto* bar = uploadInfo.value(uploadFile);
          if(bar)
          {
              bar->setMaximum(t);
              bar->setValue(r);
          }else
          {

              bar->setMaximum(0);
              bar->setValue(0);
          }

      });


      connect(uploadFile,&UploadFileHTTP::UplaodFinished,this,[this,uploadFile,freeBar](const QString& response){
          uploadInfo.remove(uploadFile);
          uploadFile->deleteLater();
          freeBar->setValue(0);
          freeBar->setMaximum(0);
          ui->textEditLogLocal->append(response);
      });

      uploadFile->UploadFile(pathUploadFile,"C:/Users/Gera/Desktop",false);


  });

  connect(ui->btnMove,&QPushButton::clicked,this,[this](){

      ui->textEditLogLocal->append("Инициализирую POST Запрос");
      FileManager choicerFile;
      QString pathUploadFile = choicerFile.chooseFile();

      // QModelIndex indexPathServer = ui->treeViewServerFS->currentIndex();
      // if(indexPathServer.column() != 0) return;

      // QString nameFile = modelTree->data(indexPathServer).toString();
      // QString tempPath = jsonManager->getFullPath(nameFile);
      // QDir dir(tempPath);
      // dir.cdUp();
      // QString fullpath = dir.absolutePath();

      auto* uploadFile = new UploadFileHTTP(infoServer);

      qDebug() << pathUploadFile;

      QProgressBar* freeBar = nullptr;

      if (ui->progressBarUploadOne->value() == 0) freeBar = ui->progressBarUploadOne;
      else if (ui->progressBarUploadTwo->value() == 0) freeBar = ui->progressBarUploadTwo;
      else if (ui->progressBarUploadThree->value() == 0) freeBar = ui->progressBarUploadThree;

      uploadInfo.insert(uploadFile,freeBar);

      connect(uploadFile,&UploadFileHTTP::UploadProgress,this,[this,uploadFile](qint64 r,qint64 t){
          auto* bar = uploadInfo.value(uploadFile);
          if(bar)
              {
              bar->setMaximum(t);
              bar->setValue(r);
          }else
              {

              bar->setMaximum(0);
              bar->setValue(0);
          }

      });


      connect(uploadFile,&UploadFileHTTP::UplaodFinished,this,[this,uploadFile,freeBar](const QString& response){
          uploadInfo.remove(uploadFile);
          uploadFile->deleteLater();
          freeBar->setValue(0);
          freeBar->setMaximum(0);
          ui->textEditLogLocal->append(response);
      });

      uploadFile->UploadFile(pathUploadFile,"C:/Users",true);

  });


  connect(ui->btnCopy,&QPushButton::clicked,[this](){

          ui->textEditLogLocal->append("Инициализирую GET Запрос");

          QModelIndex indexPathServer = ui->treeViewServerFS->currentIndex();
          if(indexPathServer.column() != 0) return;

          QString nameFile = modelTree->data(indexPathServer).toString();
          QString fullPath = jsonManager->getFullPath(nameFile);

          qDebug() << "full" << fullPath << nameFile;
          // FileManager choicerFile;

          //QString savePath = choicerFile.saveFile();
          DownLoaderHTTP* downLoader = new DownLoaderHTTP(infoServer);



          QProgressBar* freeBar = nullptr;
          if (ui->progressBarDownloadOne->value() == 0) freeBar = ui->progressBarDownloadOne;
          else if (ui->progressBarDownloadTwo->value() == 0) freeBar = ui->progressBarDownloadTwo;
          else if (ui->progressBarDownloadThree->value() == 0) freeBar = ui->progressBarDownloadThree;

          if (!freeBar) return;

          downloadInfo.insert(downLoader, freeBar);

          connect(downLoader,&DownLoaderHTTP::downloadProgress,this,[this,downLoader](qint64 r, qint64 t){
              QProgressBar* bar = downloadInfo.value(downLoader);
              if (bar) {
                  if (t > 0) {
                      bar->setMaximum(t);
                      bar->setValue(r);
                  } else {
                      bar->setMaximum(0);
                      bar->setMinimum(0);
                  }
              }
          });

          connect(downLoader,&DownLoaderHTTP::finish,this,[this,downLoader,freeBar](){
              downloadInfo.remove(downLoader);
            downLoader->deleteLater();
              freeBar->setValue(0);

            });

          downLoader->DownloadFile(fullPath, "C:/Users/Gera/Desktop");


  });

  connect(ui->btnDeleteServer,&QPushButton::clicked,[this](){
      ui->textEditLogLocal->append("Инициализирую GET Запрос");

      QModelIndex indexPathServer = ui->treeViewServerFS->currentIndex();
      if(indexPathServer.column() != 0) return;

      QString nameFile = modelTree->data(indexPathServer).toString();
      QString fullPath = jsonManager->getFullPath(nameFile);

      QUrl url;
      url.setScheme("https");
      url.setHost(infoServer->ip());
      url.setPort(infoServer->port());
      url.setPath("/api/DeleteObject");

      QUrlQuery query;
      query.addQueryItem("path", fullPath);
      url.setQuery(query);

      qDebug() << fullPath << "fulpath" << url;
      ApiService s;

    s.GetDeleteObject(url);


  });

  connect(ui->btnViewMetaDataServerFile,&QPushButton::clicked,[this](){
      QEventLoop loop;

      ui->textEditLogLocal->append("Инициализирую GET Запрос");

      QModelIndex indexPathServer = ui->treeViewServerFS->currentIndex();
      if(indexPathServer.column() != 0) return;

      QString nameFile = modelTree->data(indexPathServer).toString();
      QString fullPath = jsonManager->getFullPath(nameFile);
      ApiService s;
      connect(&s,&ApiService::infoFileReady,this,[this,&loop](const QByteArray& arr){
          ui->textEditLogLocal->append(QString::fromUtf8(arr));
          loop.quit();
      });
      QUrl url;
      url.setScheme("https");
      url.setHost(infoServer->ip());
      url.setPort(infoServer->port());
      url.setPath("/api/InfoFile");

      QUrlQuery query;
      query.addQueryItem("path", fullPath);
      url.setQuery(query);
      ui->textEditLogLocal->append(url.toString());

      s.GetInfoFile(url);
      loop.exec();

  });

  connect(ui->btnUpdate,&QPushButton::clicked,this,[this](){

      QString tempPath = jsonManager->getFullPath();
      QDir dir(tempPath);
      dir.cdUp();
      QString fullpath = dir.absolutePath();

      qDebug()<<fullpath;
      QUrl url("https://" + infoServer->ip() + ":" + QString::number(infoServer->port()) + "/api/ListFile");

      apiHelper->GetListFileSystemServer(url,fullpath);

      connect(apiHelper.get(),&ApiService::finishedListFile,this,[this](){
          ui->textEditLogLocal->append("Обновлено успешно");
      });
  });
}

HTTPserverForm::~HTTPserverForm()
{
    delete ui;
}
void HTTPserverForm::init()
{
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    QSslConfiguration::setDefaultConfiguration(sslConfig);

    modelTree = new QStandardItemModel(this);

    ui->treeViewServerFS->setModel(modelTree);
    ui->treeViewServerFS->setRootIsDecorated(false);
    ui->treeViewServerFS->setItemsExpandable(false);
    ui->treeViewServerFS->setEditTriggers(QAbstractItemView::NoEditTriggers);

    infoServer = std::make_shared<ConnectionInfoHTTP>();
    infoServer->setIp("127.0.0.1");
    infoServer->setPort(443);

    jsonManager = std::make_unique<JsonDataProcessorHTTP>();
    utilsHelperHttp = std::make_unique<UtilsHTTP>();
    apiHelper = std::make_unique<ApiService>();

    QUrl url("https://" + infoServer->ip() + ":" + QString::number(infoServer->port()) + "/api/ListFile");

    apiHelper->GetListFileSystemServer(url,"C:/Users");

    connect(apiHelper.get(),&ApiService::finishedListFile,this,[this](){
        ConstructTreeView();
    });

}


void HTTPserverForm::ConstructTreeView()
{

    qDebug() << "вошел";

    QFile file("StructFS.json");

    if (!file.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray())
        return;


    QJsonArray array = doc.array();

    modelTree->clear();

    modelTree->setHorizontalHeaderLabels({
        "Имя", "Тип", "Размер КБ"
    });

    QStandardItem *root = modelTree->invisibleRootItem();


    for (const QJsonValue &v : array)
    {
        if (!v.isObject())
            continue;

        QJsonObject obj = v.toObject();

        QString nameStr = obj["n"].toString();//имя
        QString pathStr = obj["p"].toString();// путь
        QString typeStr = obj["t"].toString(); // тип
        qint64 sizeVal = obj["s"].toVariant().toLongLong() / 1024; // рзмер


        auto *nameItem = new QStandardItem(nameStr);
        auto *typeItem = new QStandardItem(typeStr == "d" ? "Директория" : "Файл");
        auto *sizeItem = new QStandardItem(typeStr == "d" ? "" : QString::number(sizeVal));

        nameItem->setIcon(typeStr == "d" ? QIcon::fromTheme("folder")
                                         : QIcon::fromTheme("text-x-generic"));

        nameItem->setData(pathStr, Qt::UserRole);
        nameItem->setData(typeStr, Qt::UserRole + 1);

        QList<QStandardItem*> row;
        row << nameItem << typeItem << sizeItem;

        root->appendRow(row);

    }

    file.close();

}

void HTTPserverForm::on_btnRegestration_clicked()
{
    QUrl url("https://" + infoServer->ip() + ":" + QString::number(infoServer->port()) + "/api/reg");

    QJsonObject json;
    json["Login"] = ui->lineEditLoginRegister->text();
    json["Password"] = ui->lineEditPasswordRegister->text();
    json["Permission"] = ui->comboBoxPermisionRegister->currentText().toInt();

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    auto reply = manager->post(request,data);
    qDebug() << url;

    connect(reply,&QNetworkReply::finished,this,[reply,this](){
        QMessageBox::information(this,"Успешно",reply->readAll());
        reply->deleteLater();

    });

}

