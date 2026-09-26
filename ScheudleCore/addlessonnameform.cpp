#include "addlessonnameform.h"
#include "ui_addlessonnameform.h"
#include"../FileManagerCore/managerfs.h"
#include<QShortcut>

AddLessonNameForm::AddLessonNameForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddLessonNameForm)
    , schedule(std::make_unique<DataSchedule>())
    , parseJson(std::make_unique<ScheduleJsonParser>())
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayout);

    ManagerWorker w;
    w.CreateDir(".","Расписание");

    QShortcut* shortcut = new QShortcut(QKeySequence("Enter"),this);

    connect(ui->btnSave,&QPushButton::clicked,this,[this](){

        if(!ui->lineEditNamePair->text().isEmpty()){
            ui->listWidget->addItem("Название:" +ui->lineEditNamePair->text());
            ui->lineEditNamePair->setText("");
        }

    });
    connect(shortcut,&QShortcut::activated,this,[this](){

        if(!ui->lineEditNamePair->text().isEmpty()){
            ui->listWidget->addItem("Название:" + ui->lineEditNamePair->text());
            ui->lineEditNamePair->setText("");
        }

    });

    connect(ui->listWidget,&QListWidget::clicked,[this](const QModelIndex& index){
        int curr = index.row();

        QListWidgetItem* item = ui->listWidget->item(curr);

        if (item) {
            delete ui->listWidget->takeItem(curr);
        }

    });





    connect(ui->btnGo,&QPushButton::clicked,this,&AddLessonNameForm::SendLessonNameDataToJson);

    connect(ui->btnHelp,&QPushButton::clicked,this,[this](){

            QMessageBox::information(this,"📚 ARDA - Правила системы расписаний",
                "<html>"
                "<head>"
                "<style>"
                "body { font-family: 'Segoe UI', Arial; font-size: 10pt; }"
                ".header { color: #2C3E50; font-size: 12pt; font-weight: bold; text-align: center; margin-bottom: 15px; }"
                ".rule { background: #F8F9FA; padding: 10px; border-radius: 6px; margin: 8px 0; border-left: 4px solid; }"
                ".rule1 { border-color: #E74C3C; }"
                ".rule2 { border-color: #3498DB; }"
                ".title { color: #34495E; font-weight: bold; margin-bottom: 5px; }"
                ".text { color: #7F8C8D; }"
                ".tip { background: #FFFDE7; padding: 8px; border-radius: 4px; margin-top: 10px; border: 1px solid #FFEB3B; font-size: 9pt; color: #F39C12; }"
                "</style>"
                "</head>"
                "<body>"
                "<div class='header'>🎓 Основные правила работы с ARDA</div>"

                "<div class='rule rule1'>"
                "<div class='title'>1️⃣ УНИКАЛЬНОСТЬ НАЗВАНИЙ</div>"
                "<div class='text'>• Все названия пар должны быть <b>уникальными</b><br>"
                "• При добавлении дубликата система <b style='color:#C0392B;'>автоматически удаляет</b> старую запись<br>"
                "• Новая пара сохраняется с уникальным идентификатором</div>"
                "</div>"

                "<div class='rule rule2'>"
                "<div class='title'>2️⃣ УДАЛЕНИЕ ПАР</div>"
                "<div class='text'>• Для удаления <b>кликните по названию</b> пары в списке<br>"
                "• Система запросит подтверждение операции<br>"
                "• Удаленные данные можно восстановить из архива</div>"
                "</div>"

                "<div class='tip'>"
                "💡 <i>Подсказка: Для быстрого поиска используйте комбинацию Ctrl+F</i>"
                "</div>"
                "</body>"
                "</html>"
                );

    });
}

AddLessonNameForm::~AddLessonNameForm()
{
    delete ui;
}

bool AddLessonNameForm::SendLessonNameDataToJson()
{
    if(ui->listWidget->count() == 0){
        QMessageBox::warning(this,"Ошибка отправки","А пары где?");
        return false;

    }

    for(int i = 0; i<ui->listWidget->count(); i++)
    {
        QString str = std::move(ui->listWidget->item(i)->text().remove("Название:"));
        schedule->LessonsName.insert(str);
    }

    QString file = "Расписание/lesson_name.json";

    bool success = parseJson->setLessonNameCabinets(file,schedule->LessonsName);

    if(!success)
    {
        QMessageBox::warning(this,"Ошибка отправки","что то вставить данные не получислось\n\n Проверь папку Расписание по пути программы или перезапусти прогу");

        return success;
    }

    QMessageBox::information(this,"Успешно","Добавлено успешно");

    return success;


}
