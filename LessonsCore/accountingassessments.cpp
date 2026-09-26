 #include "accountingassessments.h"
#include "ui_accountingassessments.h"
#include "../IOCore/IODataHandler.h"
#include <memory>
#include "academicrecordwindow.h"
#include "../mainwindow.h"
#include "../Serializer.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QTextDocument>
#include <QFile>
#include<QInputDialog>

accountingassessments::accountingassessments(MainWindow* mainWindow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::accountingassessments)
    , backWindow(mainWindow)
    , serelizationJson(std::make_unique<SerelizerJsonManagerLessonModule<LessonAdapter>>())
     //fileManager(new FileManager(this))
    , timer(new QTimer(this))
    , handlerRequest(std::make_unique<ApiHandler>())
{
    ui->setupUi(this);
    setWindowTitle("Учет Оценок");
    setAttribute(Qt::WA_DeleteOnClose);
   // this->setLayout(ui->l);

    //initDatabase();
 //   connect(ui->btnAdd,&QPushButton::clicked,this,&accountingassessments::onAddRecord);
    connect(ui->btnAddDay,&QPushButton::clicked,this,&accountingassessments::on_btnAddDay_clicked);
    connect(ui->btnSave,&QPushButton::clicked,this,&accountingassessments::on_btnSave_clicked);
   // connect(ui->btnEditColmn,&QPushButton::clicked,this,&accountingassessments::editColumnName);
    //on_btnGetGroup_clicked();

    m_model = new LessonTableModel(0,0,this);
    m_model->setColumnCount(DEFAULT_TABLES);
    m_model->setHorizontalHeaderLabels({"ФИО", "Группа", "Преподователь","Название урока"});
    ui->tableView->setModel(m_model);
    ui->tableView->setWordWrap(true);
    ColumnLessonDelegate *delegate = new ColumnLessonDelegate(this);
    ui->tableView->setItemDelegate(delegate);




 /*   ui->checkBoxConfrimDelete->setChecked(true);

 //    QMenuBar *menuBar = new QMenuBar(this);

 //    QMenu *Function = new QMenu("Функционал", menuBar);
 //    SaveJSON = new QAction("Сохранить в Json", Function);
 //    UploadJSON = new QAction("Выгрузить из JSON", Function);
 //    Print = new QAction("Печатать в PDF", Function);

 //    Help = new QAction("Помощь", Function);
 //    Quit = new QAction("Выйти", Function);

 //    Function->addAction(SaveJSON);
 //    Function->addAction(UploadJSON);
 //    Function->addAction(Print);
 //    Function->addAction(Quit);
 //    Function->addAction(Help);

 //    menuBar->addMenu(Function);

 //    ui->verticalLayout->addWidget(menuBar);

 //    ui->tableWidget->setHorizontalHeaderLabels(JsonKeys::headerLessons);

 //    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
 //    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(150);
 //    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

 //  //serelizationJson->load(ui->tableWidget);

 //    connect(timer,&QTimer::timeout,this,[this](){
 //      serelizationJson->run(ui->tableWidget);
 //    });

 //    timer->start(30'000); //ms
  setupConnections();
 */


}


accountingassessments::~accountingassessments()
{

    //obj->SaveDateWidget(ui->tableWidget);

    delete ui;
}
bool accountingassessments::initDatabase()
{

}



void accountingassessments::onAddRecord() {

    int currentRowCount = m_model->rowCount(QModelIndex());

    bool success = m_model->insertRow(currentRowCount, QModelIndex());

    if (success) {
        ui->tableView->resizeRowsToContents();
        qDebug() << "Новая строка успешно добавлена в конец!";
    }
}

void accountingassessments::editColumnName()
{
    int currentColumn = 0;

    if (currentColumn >= 0) {
        bool ok;

        QString textDate = QInputDialog::getText(this, "Изменение даты",
                                                 "Введите новую дату (ДД.ММ.ГГГГ):",
                                                 QLineEdit::Normal, "", &ok);

        if (ok && !textDate.isEmpty()) {

            m_model->setHeaderData(currentColumn, Qt::Horizontal, textDate, Qt::EditRole);
        }
    }
}

// ok
/*
void accountingassessments::Printer()
{
    PrinterData();
}

QString accountingassessments::SetHeaderDocument()
{
    return "<tr><th>ID</th><th>ФИО</th><th>Группа</th><th>Название урока</th><th>Оценки</th><th>Марки Студента</th></tr>";
}

QString accountingassessments::SetNameDocument()
{

    return "<h2 align='center'>Отчёт по оценкам</h2>";
}

QString accountingassessments::setFormat(const QJsonObject &currObj)
{
    QString id = currObj["ID"].toString();
    QString fio = currObj["ФИО"].toString();
    QString group = currObj ["Группа"].toString();
    QString lesson = currObj["Название Урока"].toString();
    QJsonArray grades = currObj["Оценки Студента"].toArray();
    QJsonArray marks = currObj["Марки Студента"].toArray();

    QStringList grade;
    QStringList mark;
    for(const auto& c : grades)
    {
        grade << c.toString();
    }
    for(const auto&c : marks)
    {
        mark << c.toString();
    }

    QString html = QString("<tr>"
                           "<td>%1</td>"
                           "<td>%2</td>"
                           "<td>%3</td>"
                           "<td>%4</td>"
                           "<td>%5</td>"
                           "<td>%6</td>"
                           "</tr>")
                       .arg(id, fio, group,lesson, grade.join(""),mark.join(""));

    return html;
}



*/

void accountingassessments::on_btnAddDay_clicked()
{

    if (m_model->rowCount(QModelIndex()) == 0) {
        m_model->insertRows(0, 24,QModelIndex()); // Создает 24 строки «за раз»
    }

    // 2. И теперь добавляем ОДНУ колонку для нового учебного дня/даты в самый конец
    int currentColumns = m_model->columnCount(QModelIndex());
    m_model->insertColumn(currentColumns); // Добавит колонку, и на экране сразу появится сетка ячеек!

    // 3. Подгоняем высоту ячеек под наш двойной делегат
    ui->tableView->resizeRowsToContents();
}


void accountingassessments::on_btnSave_clicked()
{
}


void accountingassessments::on_btnGetGroup_clicked()
{
    qDebug() << "Кнопка нажата";

    // 1. Подключаем сигнал ДО отправки запроса
    connect(handlerRequest.get(), &ApiHandler::requestSuccess, this,
            [this](const QJsonObject &data, int id) {

                qDebug() << "Данные получены, размер:" << data.size();
                qDebug() << "Данные:" << data;

               // ui->comboBoxTables->clear();
                QJsonArray groups = data["data"].toArray();
                for (const auto &item : groups) {
                    QJsonObject obj = item.toObject();
                    QString groupName = obj["group_name"].toString();
                    QString disciplineName = obj["discipline_name"].toString();

                    // Формируем строку "Группа - Предмет"
                    QString displayText = groupName + " - " + disciplineName;

                 //   ui->comboBoxTables->addItem(displayText);
                }
            });

    connect(handlerRequest.get(), &ApiHandler::requestError, this,
            [this](const QString &error, int id) {
                qDebug() << "Ошибка:" << error;
                // Покажи сообщение пользователю
                QMessageBox::warning(this, "Ошибка", "Не удалось получить данные: " + error);
            });

    // 2. Отправляем запрос
    QJsonObject obj;
    obj["token"] = ActiveSession::instance()->getToken();
    handlerRequest->execute("/api/GetTablesTeacher", obj);

    qDebug() << "Запрос отправлен, ждём ответ...";

    // 3. Этот qDebug() выполнится СРАЗУ, ДО получения ответа
    // Это НОРМАЛЬНО!
}

