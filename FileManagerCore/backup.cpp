#include "backup.h"
#include "ui_backup.h"
#include<QListView>
#include<QStorageInfo>
#include<QGridLayout>
#include <QMessageBox>
#include"../confrimdialog.h"
#include"filemanagerdialog.h"
#include<QItemSelectionModel>
#include<QDirIterator>
#include<QtConcurrent/QtConcurrent>
BackUp::BackUp(filemanagerdialog* fmd,QWidget *parent)
    : QWidget(parent)
    , fmd_(fmd)
    , ui(new Ui::BackUp)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    model = new QFileSystemModel(this);
    model->setFilter(QDir::QDir::AllEntries);
    model->setRootPath("");
    QItemSelectionModel* selectModel = new QItemSelectionModel(model,this);
    ui->sourceB->setModel(model);
    ui->BackUpB->setModel(model);
    ui->sourceB->setSelectionModel(selectModel);
    ui->sourceB->setSelectionMode(QAbstractItemView::ExtendedSelection);

    worker = new BackupWorker();
    thread = new QThread();

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);

    ui->progressBar->setTextVisible(true);
    ui->textEdit->setReadOnly(true);


    connect(ui->BackUpB,&QListView::doubleClicked,this,[this](const QModelIndex& index){
        on_sourceB_doubleClicked(index);
    });

    connect(ui->bntBckUp,&QPushButton::clicked,this,[this](){
        ui->bntBckUp->setEnabled(false);
        QMessageBox::StandardButton confrim = QMessageBox::question(
            this,
            "Подтверждение",
            "Вы уверены что хотите выполнить резервное копирование?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (confrim == QMessageBox::Yes) {
            QModelIndex index = ui->BackUpB->rootIndex(); // куда
            QString s = model->filePath(ui->sourceB->rootIndex()); // откуда
            QString b = model->filePath(index);
            qDebug()<< s << " " << b;
            emit startOperationAll(s, b);
        } else {
            QMessageBox::information(this, "Отмена", "Действие отменено");
            ui->bntBckUp->setEnabled(true);
        }

    });
    connect(worker,&BackupWorker::unlockBTN,this,[this](){
        ui->bntBckUp->setEnabled(true);
    });
    connect(worker,&BackupWorker::logMessage,this,[this](const QString& msg){
        ui->textEdit->append(msg);
    });
    connect(worker,&BackupWorker::finishedCopy,this,[this](){
        QMessageBox::information(this,"Копирование","Копирование файло завершенно!");
        ui->bntBckUp->setEnabled(true);
    });
    connect(worker,&BackupWorker::progressChanged,ui->progressBar,&QProgressBar::setValue);
    connect(ui->btnQuit,&QPushButton::clicked,this,&BackUp::Back);


    
    connect(ui->sourceB->selectionModel(),&QItemSelectionModel::selectionChanged,this,&BackUp::onSelectionChanged);
    connect(ui->btnSelectElement,&QPushButton::clicked,this,[this](){
        QModelIndex index = ui->BackUpB->currentIndex();
        QString b = model->filePath(index);
        qDebug() << b << " " << QString::number(files_.size());
        if(files_.size() > 0)
            emit startOperationSelect(files_,b);
    });

    connect(this,&BackUp::startOperationSelect,worker,&BackupWorker::runBackUpNew);
    connect(this,&BackUp::startOperationAll,worker,&BackupWorker::runBackUp);

    worker->moveToThread(thread);
    thread->start();
}

BackUp::~BackUp()
{

    thread->quit();
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);

    delete ui;
}



void BackUp::on_sourceB_doubleClicked(const QModelIndex &index)
{
        QListView* listView = qobject_cast<QListView*>(sender());

        QFileInfo fileInfo = model->fileInfo(index);

        if(fileInfo.fileName() == ".")
        {
            listView->setRootIndex(model->index(""));
        }else if(fileInfo.fileName() == "..")
        {
            QDir dir = fileInfo.path();
            dir.cdUp();
            listView->setRootIndex(model->index(dir.absolutePath()));
        }else if(fileInfo.isDir())
        {
            listView->setRootIndex(index);
            files_.clear();
        }


}

void BackUp::Back()
{
    this->close();
    fmd_->show();
}



// == WORKER LOGIC ==
void BackupWorker::runBackUp(QString sPath, QString bPath)
{

    if (!sPath.endsWith('/')) sPath += '/';
    if (!bPath.endsWith('/')) bPath += '/';

    int counter = 0;
    QDir s(sPath);
    QDir b(bPath);

    const bool isStorage = StorageBackup(s,b);
    if(!isStorage) return;

    QFileInfoList files;

    compareDirs(s, b, files);

    int totalFiles = files.size();

    for (const QFileInfo& c : files) {
        QString pathBackUp = c.filePath().replace(sPath, bPath);


        if (c.isDir()) {
            QDir().mkpath(pathBackUp);
        } else if (c.isFile()) {
            QFileInfo destFile(pathBackUp);
            QDir().mkpath(destFile.path());

            if (QFile::exists(pathBackUp)) {
                QFile::remove(pathBackUp);
            }

            if (!QFile::copy(c.absoluteFilePath(), pathBackUp)) {
                emit logMessage("Не удалось скопировать файл:" + c.filePath());
            } else {
                emit logMessage("Скопирован:" + c.filePath());
            }

        }
        counter++;
        int progress = static_cast<int>((counter * 100.0) / totalFiles);
        emit progressChanged(progress);

    }

    emit finishedCopy();
    qDebug()<< "End BackUp";
}
// ==========
void BackupWorker::recursiveList(QDir &sDir, QFileInfoList &filesList)
{
    QFileInfoList datas = sDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs
                                             ,QDir::Name | QDir::DirsFirst);

    for(const auto & info : datas)
    {
        filesList.append(info);
        if(info.isDir() && sDir.cd(info.fileName()))
        {
            recursiveList(sDir,filesList);
            sDir.cdUp();
        }
    }
}

void BackupWorker::compareDirs(QDir &sdir, QDir &ddir, QFileInfoList& fileList)
{

// Изменения 12 03 2026 v6.4 11:32 оптимизация работы


    QDirIterator iterDirsS(sdir.absolutePath(),
                           QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs,
                           QDirIterator::NoIteratorFlags);

    

    while (iterDirsS.hasNext())
    {
        iterDirsS.next();

        QFileInfo s(iterDirsS.fileInfo());

        bool foundFile = false;
        QString targetPath = ddir.absoluteFilePath(s.fileName());

        if(QFileInfo::exists(targetPath)){

            QFileInfo d(targetPath);

            if(s.isDir() || s.lastModified() <= d.lastModified())
            {
                foundFile = true;
            }

        }

        if(!foundFile)
        {
            fileList.append(s);
        }

        if(s.isFile()) continue;

        if(foundFile)
        {
            sdir.cd(s.fileName());
            ddir.cd(s.fileName());
            compareDirs(sdir,ddir,fileList);
            sdir.cdUp();
            ddir.cdUp();

        }else
        {
            sdir.cd(s.fileName());
            recursiveList(sdir,fileList);
            sdir.cdUp();
        }

    }

}
qint64 BackupWorker::getTotalSizeMBDirsCopy(QDir &s)
{
    qint64 totalSize = 0;
    QFileInfoList files = s.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    for(const auto& c : files) {
        if (c.isDir()) {
            QDir subDir(c.absoluteFilePath());
            totalSize += getTotalSizeMBDirsCopy(subDir);
        } else {
            totalSize += c.size();
        }
    }
    return totalSize;// BYTES
}

bool BackupWorker::StorageBackup(QDir &s, QDir& b)
{
    qint64 sizeBytes = getTotalSizeMBDirsCopy(s);
    qint64 freeBytes = QStorageInfo(b.rootPath()).bytesAvailable();

    double sizeMB = static_cast<double>(sizeBytes) / (1024 * 1024);
    double freeMB = static_cast<double>(freeBytes) / (1024 * 1024);

    QString unit = " MB";
    double displaySize = sizeMB;

    if (sizeMB < 1.0 && sizeBytes > 0) {
        displaySize = static_cast<double>(sizeBytes) / 1024;
        unit = " KB";
    }

    emit logMessage("Размер копируемых данных: " + QString::number(displaySize, 'f', 2) + unit);
    emit logMessage("Свободно на диске назначения: " + QString::number(freeMB, 'f', 0) + " MB");

    if (freeBytes <= sizeBytes) {
        emit logMessage("Недостаточно места на диске!");
        emit unlockBTN();
        return false;
    }

    emit logMessage("Места достаточно. Можно копировать.");
    return true;
}

void BackUp::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
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

bool BackupWorker::runBackUpNew(const QFileInfoList& files, QString dPath)
{
    bool success = true;
    int counter = 0;


    QDir s = files.at(0).absoluteDir();
    QDir d(dPath);
    const bool isStorage = StorageBackup(s,d);
    if(!isStorage) return;

    QFileInfoList files2;
    compareDirs(s, d, files2);

    int totalFiles = files2.size();

    for (const auto& dataFile : files)
    {

        if (dataFile.isFile())
        {
            QString destFilePath = dPath + QDir::separator() + dataFile.fileName();

            if(QFile(destFilePath).exists())
            {
                if(dataFile.lastModified() > QFileInfo(destFilePath).lastModified()){
                    QFile(destFilePath).remove();
                }
            }

            if (!QFile::copy(dataFile.absoluteFilePath(), destFilePath)) {
                logger->write("Ошибка копирования файла:" + dataFile.absoluteFilePath().toStdString()
                    + " в " + destFilePath.toStdString());
                success = false;
            } else {
                logger->write("Успешно скопирован файл:" + dataFile.absoluteFilePath().toStdString()
                    + " в " + destFilePath.toStdString());
            }
        }
        else if(dataFile.isDir())
        {
            QString destDir = dPath + QDir::separator() + dataFile.fileName();
            QString sourceDir = dataFile.absoluteFilePath();

            recursiveCopy(sourceDir,destDir);


        }
        else {

            logger->write("Пропуск не файлового объекта: " + dataFile.absoluteFilePath().toStdString());
        }

        counter++;
        int progress = static_cast<int>((counter * 100.0) / files2.size());
        emit progressChanged(progress);

    }
    emit progressChanged(100);

    return success;
}



bool BackupWorker::recursiveCopy(QString& sPath, QString& dPath) {

    QDir sourceDir(sPath);
    QDir destDir(dPath);

    qDebug() << sPath << " " << dPath << " " << sourceDir.absolutePath() << " " << destDir.absolutePath();

    bool success = true;

    if (!destDir.exists()) {
        if (!destDir.mkpath(".")) {
            logger->write("Не удалось создать директорию:" + dPath.toStdString());
            return false;
        }
    }

    for (const auto& entryInfo : sourceDir.entryInfoList(QDir::Filters(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::DirsFirst))) {

        QString sourceEntryPath = sourceDir.absolutePath() + QDir::separator() + entryInfo.fileName();
        QString destEntryPath = destDir.absolutePath() + QDir::separator() + entryInfo.fileName();
        qDebug() << sourceEntryPath << " " << destEntryPath;

        if (entryInfo.isFile()) {
            if (QFile(destEntryPath).exists()) {
                if(entryInfo.lastModified() > QFileInfo(destEntryPath).lastModified())
                {
                    QFile(destEntryPath).remove();
                }
            }

            if (!QFile::copy(sourceEntryPath, destEntryPath)) {
                logger->write("Ошибка копирования файла:" + sourceEntryPath.toStdString() + " в " + destEntryPath.toStdString());
                success = false;
            } else {
                logger->write("Успешно скопирован файл:" + sourceEntryPath.toStdString() + " в " + destEntryPath.toStdString());
            }
        } else if (entryInfo.isDir()) {
            success = recursiveCopy(sourceEntryPath, destEntryPath);
        }
    }

    return success;
}
