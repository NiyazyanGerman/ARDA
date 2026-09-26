#include "texpanel.h"
#include "ui_texpanel.h"
#include<QSqlError>
#include<QInputDialog>
#include<QMenu>
#include<QProcess>
#include<QDateTime>
#include<QDir>
#include<QFileDialog>
#include<QMessageBox>
TexPanel::TexPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TexPanel)

{
    ui->setupUi(this);
    setWindowTitle("AKUMA ADMINISTARTOR");

    setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/ico.ico"));
    handlerDb = std::make_unique<QDataBaseHandler>();
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested, this, &TexPanel::onTreeContextMenu);

    connect(ui->editPass,&QLineEdit::textChanged,this,[this](const QString& pass) {
        prop.password = pass;
    });

    connect(ui->editHost, &QLineEdit::textChanged, this, [this](const QString& text) {
        prop.host = text;
    });
    connect(ui->editDb, &QLineEdit::textChanged, this, [this](const QString& text) {
        prop.dbName = text;
    });
    connect(ui->editUser, &QLineEdit::textChanged, this, [this](const QString& text) {
        prop.user = text;
    });

    connect(ui->editPort, &QLineEdit::textChanged, this, [this](const QString& text) {
        prop.port = text.toInt();
    });

    connect(handlerDb.get(),&QDataBaseHandler::getInfo,this,[this](const QString& log){
        ui->textEdit->append(log+"\n");
    });

}

TexPanel::~TexPanel()
{
    delete ui;
}

void TexPanel::on_btnConnect_clicked()
{
    qDebug() << "вошел";

    auto succesConn = handlerDb->create(prop);

    if(succesConn)
    {
        m_db = QSqlDatabase::database(CONNECTION_DB_HANDLE);
        model = new QSqlTableModel(this,m_db);
        shemes =  handlerDb->getSchemes();
        viewDB();
        return;
    }

    return;
}


void TexPanel::viewDB()
{

    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(0);
    ui->treeWidget->setHeaderLabels(QStringList() << prop.dbName);

    for (auto it = shemes.constBegin(); it != shemes.constEnd(); ++it) {
        const QString& schemaName = it.key();
        const QStringList& tables = it.value();

        QTreeWidgetItem* schemaItem = new QTreeWidgetItem(ui->treeWidget);
        schemaItem->setText(0, schemaName);

        for (const QString& tableName : tables) {
            QTreeWidgetItem* tableItem = new QTreeWidgetItem(schemaItem);
            tableItem->setText(0, tableName);
        }
    }

    ui->treeWidget->expandAll();
}


void TexPanel::on_btnLoadCurrentTable_clicked()
{


   QTreeWidgetItem* currentItem =  ui->treeWidget->currentItem();

    if (currentItem) {
        QString currentTable = currentItem->text(0);

        qDebug() << currentTable;
        QTreeWidgetItem *rootItem = currentItem;
        while (rootItem->parent() != nullptr) {
            rootItem = rootItem->parent();
        }

        QString rootName = rootItem->text(0);

        model->setTable(rootName + "." + currentTable);

        if (!model->select()) {
            qDebug() << "Ошибка загрузки данных:" << model->lastError().text();
        }


        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->show();

    }
}


void TexPanel::on_btnNewScheme_clicked()
{
    QString name = QInputDialog::getText(this,"Create Shema","Name Shema");
    handlerDb->createShema(name);
}


void TexPanel::on_btnRefersh_clicked()
{
    shemes =  handlerDb->getSchemes();
    viewDB();
    return;
}

void TexPanel::onTreeContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);
    if (!item) {
        return;
    }
    QMenu menu;

    QAction *remove = menu.addAction("Удалить");
    QAction *rename = menu.addAction("Переименовать");

    QAction *selectedAction = menu.exec(ui->treeWidget->mapToGlobal(pos));

    if (selectedAction == rename) {

    }
    else if (selectedAction == remove) {
        if (item->parent() == nullptr) {
            handlerDb->removeShema(item->text(0));
            qDebug() << "Это корень дерева";
        } else {
            handlerDb->removeTable(item->text(0));
            qDebug() << "Это плод, у него есть родитель";
        }
    }
}


void TexPanel::on_btnAddRow_clicked()
{
    int row = model->rowCount();
    model->insertRow(row);
}


void TexPanel::on_btnRemoveRow_clicked()
{
    int currentRow = ui->tableView->currentIndex().row();

    if (currentRow >= 0) {
        model->removeRow(currentRow);
    }
}


void TexPanel::on_btnSave_clicked()
{
    model->submitAll();
}



void TexPanel::on_btnBackUp_clicked()
{

    QString path = QFileDialog::getExistingDirectory(this);

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString backupFile = QString(path + "/backup_%1.backup").arg(timestamp);

    QString pass = QInputDialog::getText(this,"BackUp","Enter password");
    QProcess process;

    process.setEnvironment({QString("PGPASSWORD=%1").arg(pass)});


    QStringList arguments = {
        "-U", "postgres",
        "-F", "c",
        "-v",
        "-h", "localhost",
        "-p", "5432",
        "--schema=public",
        "-f", backupFile,
        "postgres"
    };

    process.start("pg_dump", arguments);
    process.waitForFinished(60000);

    qint64 size = QFile(backupFile).size();


    if (size == 0) {
        ui->textEdit->append("Error:" + process.readAllStandardError());
    }
}

void TexPanel::on_btnAddTable_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if (!item || item->parent() != nullptr) {
        QMessageBox::warning(this, "Создание таблицы", "Выберите корректную схему (корневой элемент)");
        return;
    }

    qDebug() << item->text(0);

    bool ok;
    QString tableName = QInputDialog::getText(this, "Создание таблицы", "Enter Name:", QLineEdit::Normal, "", &ok);
    if (!ok || tableName.trimmed().isEmpty()) return;
    ConstructorTable* table = new ConstructorTable(this);

    table->setAttribute(Qt::WA_DeleteOnClose);

    connect(table, &ConstructorTable::cretaeTableReady, this, [this, schemaName = item->text(0), tableName](const QList<TableColumn>& usersColumns) {
        bool success = handlerDb->CreateTable(schemaName, tableName, usersColumns);

        if (success) {
            QMessageBox::information(this, "Успех", "Таблица успешно создана!");
        }
    });

    // 5. ОТОБРАЖЕНИЕ ОКНА
    table->show();
    table->activateWindow(); // Выводим на передний план
}
