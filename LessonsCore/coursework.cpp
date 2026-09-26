#include "coursework.h"
#include "ui_coursework.h"
#include"../IOCore/IODataHandler.h"
#include"../mainwindow.h"
#include"../JsonKeys.h"
#include"../Validator.h"
#include<QPrinter>
#include<QPrintDialog>
#include<QPainter>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QByteArray>
#include"../FileChooicer.h"
#include<QAction>
#include<QMenu>
#include<QMenuBar>
#include<QWidgetAction>
#include"../confrimdialog.h"

Coursework::Coursework(MainWindow* mainWin, QWidget *parent)
    : ITablesManager(parent)
    , ui(new Ui::Coursework)
    , main(mainWin)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    setWindowIcon(QIcon("C:/Users/Gera/Documents/ARDA/Course.png"));
    isd = std::make_unique<ImportSaveData>();
    ser = std::make_unique<SerializerData>();

    fileManager  = new FileManager(this);

    ser->DataSerelizationCourse(ui->tableWidget,this);
    ui->lineEditFind->setPlaceholderText("Поиск по имени...");

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *moreMenu = new QMenu("Прочее", menuBar);
   confirmCheckBox = new QCheckBox("Включить Подтверждения",this);
    OnOffSerelization = new QCheckBox("Включит Серелизацию",this);
    QWidgetAction *checkBoxAction = new QWidgetAction(moreMenu);
    QWidgetAction* checkBoxSerAction = new QWidgetAction(moreMenu);
    checkBoxAction->setDefaultWidget(confirmCheckBox);
    checkBoxSerAction->setDefaultWidget(OnOffSerelization);
    moreMenu->addAction(checkBoxAction);
    moreMenu->addAction(checkBoxSerAction);
    menuBar->addMenu(moreMenu);


    connect(ui->btnAdd,&QPushButton::clicked,this,&Coursework::addRow);
    connect(ui->btnBack,&QPushButton::clicked,this,&Coursework::BackMenu);
    connect(ui->btnSave,&QPushButton::clicked,this,[=](){
         isd->SaveDateWidget(ui->tableWidget);
        isd->saveToJsonCourseWork(ui->tableWidget,this);
    });
    connect(ui->btnUpload,&QPushButton::clicked,this,[=]()
            {
        isd->loadFromCourseWork(ui->tableWidget,this);
    });
    connect(ui->btnPrinter,&QPushButton::clicked,this,&Coursework::PrinterData);
    connect(ui->bntDelete,&QPushButton::clicked,this,&Coursework::DeleteRow);
    connect(ui->btnClear,&QPushButton::clicked,this,&Coursework::ClearTable);
    connect(ui->lineEditFind,&QLineEdit::textChanged,this,&Coursework::findName);

    connect(OnOffSerelization,&QCheckBox::checkStateChanged,this,[=](int state){
        if(state == Qt::Checked)
        {
            ser->DataSerelizationCourse(ui->tableWidget,this);
        }
    });

    // connect(ui->btnSaveDate,&QPushButton::clicked,this,[=](){
    //     isd->SaveDateWidget(ui->tableWidget);
    // });
}

Coursework::~Coursework()
{
    delete ui;
}

void Coursework::BackMenu()
{
    isd->SaveDateWidget(ui->tableWidget);
    main->show();
    this->close();
}

void Coursework::PrinterData()
{
    ITablesManager::PrinterData();
}
void Coursework::addRow()
{


    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    for(int col = 0; col < 8; col++)
    {
        if(col == 6)
        {
            QComboBox* comBox = new QComboBox(this);
            comBox->addItems(Stages::StageList);
            ui->tableWidget->setCellWidget(row, col, comBox);
        }
        else if(col == 8)
        {
            QDateEdit* DateEdit = new QDateEdit(QDate::currentDate(), this);
            ui->tableWidget->setCellWidget(row, col, DateEdit);
            qDebug() << "DateEdit created at col 8";
        }
        else {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(""));
        }
    }
}


void Coursework::DeleteRow()
{
    ConfrimDialog dlg(this);
    if(confirmCheckBox->checkState() == Qt::Checked)
    {
        if(dlg.exec() == QDialog::Accepted)
        {

            int select = ui->tableWidget->currentRow();
            if(!(select > ui->tableWidget->rowCount() || select <0))
            {
                ui->tableWidget->removeRow(select);
            }

        }
        else
        {
            QMessageBox::warning(this,"Error", " Не выбрана строка для удаления или она некорректная");
            return;
        }
    }
    else
    {
        int select = ui->tableWidget->currentRow();
        if(!(select > ui->tableWidget->rowCount() || select <0))
        {
            ui->tableWidget->removeRow(select);
        }
        else
        {
            QMessageBox::warning(this,"Error", " Не выбрана строка для удаления или она некорректная");
            return;
        }
    }
}

void Coursework::ClearTable()
{
    ConfrimDialog dlg(this);
    if(confirmCheckBox->checkState() == Qt::Checked)
    {
        if(dlg.exec() == QDialog::Accepted)
        {
            if(ui->tableWidget->rowCount() > 0)
            {
            ui->tableWidget->setRowCount(0);
            }else
            {
                return;
            }
        }

    }
    else
    {
        if(ui->tableWidget->rowCount() > 0)
        {
            ui->tableWidget->setRowCount(0);
        }else
        {
            return;
        }

    }
}


void Coursework::findName(const QString &name)
{
    ui->tableWidget->clearSelection();

    bool found = false;
    for(int i =0; i<ui->tableWidget->rowCount();i++)
    {
        for(int j =0; j<ui->tableWidget->columnCount();j++)
        {
            QTableWidgetItem* item = ui->tableWidget->item(i,j);

            if(item)
            {
                if(item->text().contains(name,Qt::CaseInsensitive))
                {
                    found = true;
                    ui->tableWidget->setCurrentItem(item);
                    break;

                }
            }
        }
        if(found) break;
    }

    if(!found)
    {
        QMessageBox::warning(this,"Поиск", "Нечего не найдено");
    }

    return;
}




QString Coursework::SetHeaderDocument()
{
    return "<tr><th>ID</th><th>ФИО</th><th>Зачетная книжка</th><th>Группа</th><th>Тема</th><th>Куратор/Руководитель</th><th>Этап Работы</th><th>Оценка</th><th>Дата Сдачи</th></tr>";
}

QString Coursework::SetNameDocument()
{
    return "<h2 align='center'>Отчёт по Курсовой</h2>";
}

QString Coursework::setFormat(const QJsonObject &obj)
{

    QString html;

    QString ID = obj[JsonKeys::ID].toString();
    QString fio = obj[JsonKeys::FIO].toString();
    QString group = obj[JsonKeys::Group].toString();
    QString testBook = obj[JsonKeys::TestBook].toString();
    QString grade = obj[JsonKeys::Grade].toString();
    QString Theme = obj[JsonKeys::Theme].toString();
    QString Curator = obj[JsonKeys::Curator].toString();
    QString DateSTR = obj[JsonKeys::Date].toString();
    QString Stage = obj[JsonKeys::StageWork].toString();

    html = QString("<tr>"
                    "<td>%1</td>"
                    "<td>%2</td>"
                    "<td>%3</td>"
                    "<td>%4</td>"
                    "<td>%5</td>"
                    "<td>%6</td>"
                    "<td>%7</td>"
                    "<td>%8</td>"
                    "<td>%9</td>"
                    "</tr>")
                .arg(ID, fio,testBook,group,Theme,Curator,Stage,grade,DateSTR);

    return html;
}

