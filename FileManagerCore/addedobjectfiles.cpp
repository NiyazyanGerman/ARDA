#include "addedobjectfiles.h"
#include "ui_addedobjectfiles.h"
#include <QProcess>
#include <QMessageBox>
#include <QFileInfo>
#include "../LogicOperation.h"
#include<QDesktopServices>
#include<QInputDialog>
#include"filemanagerdialog.h"

AddedObjectFiles::AddedObjectFiles(filemanagerdialog *fmd, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddedObjectFiles)
    , fmd_(fmd)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setFilter(QDir::QDir::AllEntries);
    model->setRootPath("");
    QItemSelectionModel* selectModel = new QItemSelectionModel(model,this);
    ui->listView->setModel(model);
    ui->listView->setSelectionModel(selectModel);
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(ui->listView,&QListView::doubleClicked,this,&AddedObjectFiles::on_listView_doubleClicked);
    connect(ui->listView->selectionModel(),&QItemSelectionModel::selectionChanged,this,&AddedObjectFiles::onSelectionChanged);
    connect(ui->btnZIP,&QPushButton::clicked,this,&AddedObjectFiles::zipData);
    connect(ui->btnQuit,&QPushButton::clicked,this,&AddedObjectFiles::backMenu);
}

AddedObjectFiles::~AddedObjectFiles()
{
    delete ui;
}

void AddedObjectFiles::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QModelIndexList indexes = selected.indexes();
    QString filePath;

    for(const auto& index : indexes)
    {
        if(index.column() == 0)
        {
            filePath = model->filePath(index);
            QFileInfo info(filePath);
            if(info.fileName() != "." && info.fileName() != "..")
            {
                files_.append(info);
            }
        }
    }

    QModelIndexList delIndexes = deselected.indexes();

    for(const auto& index : delIndexes)
    {
        if(index.column() == 0)
        {
            filePath = model->filePath(index);
            QFileInfo info(filePath);
            if(info.fileName() != "." && info.fileName() != "..")
            {
                files_.removeOne(info);
            }
        }
    }


}
bool AddedObjectFiles::zipData()
{

    if(files_.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Выбирете файлы & папки для архивации");
        return false;
    }




    QModelIndex index = ui->listView->currentIndex();
    QFileInfo InfoSavePath(model->filePath(index));
    QString SavePath = InfoSavePath.absolutePath();

    QString zipFileName = QInputDialog::getText(
        this,
        "Введите название архива",
        "Имя:",
        QLineEdit::Normal,
        "",
        nullptr
    );

    QString path7Z = QInputDialog::getText(
        this,
        "Введите Путь до 7-zip(Для установки пароля)",
        "Имя:",
        QLineEdit::Normal,
        "",
        nullptr
    );

    QFileInfo pathSevenZip(path7Z + QDir::separator() + "7z.exe");

    if(!pathSevenZip.exists() && !pathSevenZip.isFile())
        QMessageBox::information(this,"Ошибка","Не удалось найти 7z");

    QString sevenZip = pathSevenZip.absolutePath();

    QString password = QInputDialog::getText(
        this,
        "Введите Пароль(необязательно)",
        "Пароль:",
        QLineEdit::Normal,
        "",
        nullptr
    );

    QStringList paths;
    foreach(const QFileInfo &fi, files_) {
        paths << fi.absoluteFilePath();
    }

    QString archiveObject = paths.join(" ");
    QString command;

    if(password.isEmpty()) {
        command =  "\"" +sevenZip + "\""  + " a \"" + SavePath + "/" + zipFileName + "\" " + archiveObject;
    } else {
        command = "\"" +sevenZip + "\"" + " a -p" + password + " \"" + SavePath + "/" + zipFileName + "\" " + archiveObject;
    }
    QProcess process;
    process.start(command);
    if(!process.waitForStarted()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось запустить 7-Zip");
        return false;
    }

    if(!process.waitForFinished(30000)) {
        process.kill();
        QMessageBox::critical(this, "Ошибка", "Превышено время выполнения");
        return false;
    }

    if(process.exitCode() == 0) {
        QMessageBox::information(this, "Успех", "Архив успешно создан:\n" + SavePath + "/" + zipFileName);
        return true;
    } else {
        QString error = process.readAllStandardError();
        QMessageBox::critical(this, "Ошибка", "Ошибка при создании архива:\n" + error);
        return false;
    }
}



void AddedObjectFiles::on_listView_doubleClicked(const QModelIndex &index)
{
    QFileInfo info(model->filePath(index));

    setWindowTitle(info.absolutePath());

    if(info.fileName() == ".")
    {
        ui->listView->setRootIndex(model->setRootPath("."));
    }
    else if(info.fileName() == "..")
    {
        QDir dir = info.path();
        dir.cdUp();
        ui->listView->setRootIndex(model->index(dir.absolutePath()));
    }
    else if(info.isDir())
    {
        ui->listView->setRootIndex(index);
    } else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));
    }
}

void AddedObjectFiles::backMenu()
{
    fmd_->show();
    this->close();
}
