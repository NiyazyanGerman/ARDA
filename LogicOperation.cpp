#include"LogicOperation.h"
#include"StudentManagerCore/ManagerStudent.h"
#include"Serializer.h"
#include"NotificationCORE/telegramnotifier.h"

bool LogicOperation::handleAddStudent(const QString &file, const QString &data,QLineEdit* LineEditClear,QWidget* parent)
{
    auto logger = Log::create("AddStudentLog.txt");
    Stud manager(logger);

    try
    {
        QFile ofile(file);
        Validator::isFileValid(ofile,ModeValidator::WriteFile);
    }
    catch(const std::invalid_argument &e)
    {
        logger->write(e.what());
        return false;
    }

    bool success = manager.uploadDataToFile(file,data);

    if (success) {
        QMessageBox::information(parent, "Менеджeр", "Успешно добавлен!");
        LineEditClear->clear();
    } else {
        QMessageBox::warning(parent, "Менеджр", "Не удалось сохранить в файл (проблема с созданием файла)");
        QMessageBox::warning(parent, "Менеджр", "Укажите другой .txt");
        return false;
    }

    return true;
}

QString LogicOperation::handlePriviewFile(const QString &file, QTextEdit *display, QWidget *parent)
{
    auto logger = Log::create("PriviewLog.txt");
    Stud manager(logger);

    try
    {
        QFile ofile(file);
        Validator::isFileValid(ofile,ModeValidator::ReadFile);
    }
    catch(const std::invalid_argument&e)
    {
        logger->write(e.what());
        return "";
    }

    QString show = manager.PrintSortStud(file);


    if (show.isEmpty()) {
        display->setPlainText("Файл пустой или не удалось прочитать.");
    } else {
        display->setPlainText(show);
    }

    return show;
}

bool LogicOperation::handleSortStudent(const QString &file, const QString &saveFile, QWidget *parent)
{
    auto logger = Log::create("SortLogStudent.txt");
    Stud manager(logger);

    try
    {
        QFile ofile(file);
        QFile ifile(saveFile);
        Validator::isFileValid(ofile,ModeValidator::ReadFile);
        Validator::isFileValid(ifile,ModeValidator::WriteFile);
    }
    catch(const std::invalid_argument&e)
    {
        logger->write(e.what());
        return false;
    }

    bool success = manager.SortStudent(saveFile,file);


    if (success) {
        QMessageBox::information(parent, "Сортировка", "Сортировка прошла успешно!");
    } else {
        QMessageBox::warning(parent, "Сортировка", "Не известная ошибка перезапустите программу :(");
        QMessageBox::warning(parent, "Сортировка", " Укажите другой .txt");
        return false;
    }

    return true;

}

void LogicOperation::handleDeleteStudent(const QString &file, const QString &name, const QString &score, QWidget *parent)
{
    qDebug() << "init d";
    auto logger = Log::create("LogDeleted.txt");
    Stud manager(logger);


    try {
        QFile ifile(file);
        Validator::isFileValid(ifile, ModeValidator::ReadFile);
    } catch (std::invalid_argument& e) {
        logger->write(e.what());
        QMessageBox::warning(parent, "Ошибка валидации файла", e.what());
        return;
    }

    std::optional<double> scoreOpt;

    try
    {
        scoreOpt = Validator::ValidOptional(score);
    }catch (std::invalid_argument& e) {
        logger->write(e.what());
        QMessageBox::warning(parent, "Ошибка валидации балла", e.what());
        return;
    }


    bool deleted = manager.DeleteStudentFromFile(file,name,scoreOpt);


    if (deleted) {
        QMessageBox::information(parent, "Удаление", QString("Студент удален: %1").arg(name));
    } else {
        QMessageBox::warning(parent, "Удаление", QString("Студент не удален: %1").arg(name));
    }

}

void LogicOperation::handleFindStudent(const QString &file, const QString &name, const QString &score, QWidget *parent)
{
    auto logger = Log::create("LogFind.txt");
    Stud manager(logger);

    std::optional<double> scoreOpt;

    try{
        QFile ifile(file);
        Validator::isFileValid(ifile,ModeValidator::ReadFile);
    }catch(std::invalid_argument& e)
    {
        logger->write(e.what());
        QMessageBox::warning(parent, "Ошибка валидации файла", e.what());
        return;
    }

    try{
        scoreOpt = Validator::ValidOptional(score);
    }catch(std::invalid_argument& e)
    {
        logger->write(e.what());
        QMessageBox::warning(parent, "Ошибка валидации балла", e.what());
        return;
    }

    bool found = manager.findStudent(file,name,scoreOpt);


    if (found) {
        QMessageBox::information(parent, "Найти", QString("Студент найден: %1").arg(name));
    } else {
        QMessageBox::warning(parent, "Найти", QString("Студент не найден: %1").arg(name));
    }



}

std::optional<double> LogicOperation::handleAutoTestStudent(QTableWidgetItem* checkItem, QList<int>& averageScore)
{

    if (!checkItem) return std::nullopt;
    if(checkItem->checkState() != Qt::Checked) return std::nullopt;
    if (averageScore.isEmpty()) return std::nullopt;
    int sum = 0;

    for(const int numBEAR : averageScore) sum+= numBEAR;

    double result = static_cast<double>(sum) / averageScore .size();

    averageScore.clear();

    return result;

}

double LogicOperation::converterTOnumberInString(const QString &number)
{
    double Num;
    bool isNum = false;
    Num = number.toDouble(&isNum);

    return Num;
}

void LogicOperation::FillSettingsTable(QSettings &settings, QTableWidget* t)
{
    for(int row=0;row< t->rowCount();row++)
    {

        for(int col=0;col< t->columnCount();col++)
        {
            QString key = QString("row_%1_col_%2").arg(row).arg(col);
            QTableWidgetItem* item = t->item(row, col);
            if (!item) continue;

            QString header = t->horizontalHeaderItem(col) ? t->horizontalHeaderItem(col)->text() : "";

            if (header == "Автомат" && (item->flags() & Qt::ItemIsUserCheckable)) {
                settings.setValue(key, static_cast<int>(item->checkState()));
            } else {
                settings.setValue(key, item->text());
            }
        }
    }
}

void LogicOperation::FillTableWidget(QSettings &settings, QTableWidget *t,int row,int column)
{
    t->blockSignals(true);

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column;j++)
        {
            QString key = QString("row_%1_col_%2").arg(i).arg(j);

            QVariant value = settings.value(key);
            qDebug() << key<< " Валью: "  << value;

            QTableWidgetItem* item = t->item(i,j);
            QString header = t->horizontalHeaderItem(j) ? t->horizontalHeaderItem(j)->text() : "";


            if (!item) {
                item = new QTableWidgetItem();
                t->setItem(i, j, item);
            }

            if (header == "Автомат") {
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                item->setCheckState(static_cast<Qt::CheckState>(settings.value(key, Qt::Unchecked).toInt()));
            }  else {
                item->setText(value.toString());
            }

        }
    }

    t->blockSignals(false);
}

void LogicOperation::FillTableWidgetCourse(QWidget* p ,QSettings &settings, QTableWidget *t,QString& app)
{

    for (int row = 0; row < t->rowCount(); ++row) {

        for (int col = 0; col < t->columnCount(); ++col) {
            QString key = QString("row_%1_col_%2").arg(row).arg(col);
            QString header = t->horizontalHeaderItem(col)->text();
            QVariant value = settings.value(key);


            if (header == JsonKeys::Date)
            {

                QDateEdit* dateEdit = new QDateEdit(p);
                QDate date = QDate::fromString(value.toString(), "dd.MM.yyyy");
                dateEdit->setDate(date);
                t->setCellWidget(row, col, dateEdit);

                QObject::connect(dateEdit, &QDateEdit::editingFinished, p, [=,&settings]() {
                    QSettings temp(SerelizationData::ORG_NAME, app);
                    temp.setValue(key, dateEdit->date().toString("dd.MM.yyyy"));
                });

            }
            else if(header == JsonKeys::StageWork)
            {
                QComboBox* comboBox = new QComboBox(p);
                comboBox->addItems(Stages::StageList);
                comboBox->setCurrentText(value.toString());
                t->setCellWidget(row,col,comboBox);

                QObject::connect(comboBox, &QComboBox::currentTextChanged, p, [=]() {
                    QSettings temp(SerelizationData::ORG_NAME, app);
                    temp.setValue(key, comboBox->currentText());
                });
            }
            else{
                QTableWidgetItem* item = new QTableWidgetItem(value.toString());
                t->setItem(row,col,item);
            }


        }
    }
}

void LogicOperation::FillTableWidgetDiploma(QWidget *p, QSettings &settings, QTableWidget *t,QString &app)
{
    for(int row = 0; row< t->rowCount(); ++row)
    {
        for(int col = 0; col <t->columnCount(); ++col)
        {
            QString key = QString("row_%1_col_%2").arg(row).arg(col);
            QString header = t->horizontalHeaderItem(col)->text();
            QVariant value = settings.value(key);

            if(header == JsonKeys::Date)
            {
                QDateEdit* dateEdit = new QDateEdit(p);
                QDate date = QDate::fromString(value.toString(), "dd.MM.yyyy");
                dateEdit->setDate(date);

                t->setCellWidget(row,col,dateEdit);

                QObject::connect(dateEdit, &QDateEdit::dateChanged, p, [=](const QDate &d){
                    QSettings mainS(SerelizationData::ORG_NAME, app);
                    mainS.setValue(key, d.toString("dd.MM.yyyy"));
                });

            }
            else if(header == JsonKeys::StageWork)
            {
                QComboBox* combo = new QComboBox(p);
                combo->addItems(Stages::StageListDiplom);
                combo->setCurrentText(value.toString());

                t->setCellWidget(row,col,combo);


                QObject::connect(combo, &QComboBox::currentTextChanged, p, [=]() {
                    QSettings mainS(SerelizationData::ORG_NAME, app);
                    mainS.setValue(key, combo->currentText());
                });
            }
            else
            {
                QTableWidgetItem* item = new QTableWidgetItem(value.toString());
                if(item){
                    t->setItem(row,col,item);
                }else
                {
                    continue;
                }
            }

        }
    }
}

QJsonArray LogicOperation::rewriteFile(QFile &file)
{

    if(!file.exists()) return QJsonArray();

    ValidFile(file,ModeValidator::ReadFile);

    QByteArray bArr =  file.readAll();
    QJsonParseError error;
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(bArr,&error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << error.errorString();
        return QJsonArray();
    }

    if (!doc.isArray()) {
        qDebug() << "JSON не садержит массив ";
        return QJsonArray();
    }

    QJsonArray arr = doc.array();

    ValidFile(file,ModeValidator::DeleteWrite);
    file.close();

    return arr;
}



QFileInfoList LogicOperation::recursiveTravelDirs(const QString &path)
{
    QFileInfoList tempList;
    QDir dir(path);

    for(const auto& c : dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
    {
        if(c.isDir() && !c.isSymLink())
        {
            tempList.append(recursiveTravelDirs(c.absoluteFilePath()));
        }
        else
        {
          tempList.append(c);
        }
    }

    return tempList;
}

void LogicOperation::validNums(QString &values)
{
    QString newData;

    for(const QChar& c : values)
    {
        if(c.isNumber())
        {
            newData += c;
        }
    }
    values.clear();
    values = newData;
}
bool LogicOperation::createTableDBfromTYPE(const DatabaseTypes type,const QSqlDatabase& db)
{
    QString sql;

    switch (type) {
    case DatabaseTypes::LESSON:
        sql = "CREATE TABLE IF NOT EXISTS lessons ("
              "\"ID\" TEXT PRIMARY KEY, "
              "\"ФИО\" TEXT NOT NULL, "
              "\"Группа\" TEXT, "
              "\"Название Урока\" TEXT, "
              "\"Оценки Студента\" TEXT, "
              "\"Марки Студента\" TEXT)";
        break;
    case DatabaseTypes::ACADEMIC:
        sql = "CREATE TABLE IF NOT EXISTS academic ("
              "\"ID\" TEXT PRIMARY KEY, "
              "\"ФИО\" TEXT NOT NULL, "
              "\"Группа\" TEXT, "
              "\"Зачетная книжка\" TEXT, "
              "\"Название предмета\" TEXT, "
              "\"Оценки Студента\" TEXT, "
              "\"Марки Студента\" TEXT, "
              "\"Автомат\" TEXT, "
              "\"Оценка за зачет\" TEXT)";
        break;
    default:
        break;
    }

    QSqlQuery query(db);
    return query.exec(sql);
}
QString LogicOperation::nameFileDb(DatabaseTypes type)
{
    switch (type) {
    case DatabaseTypes::LESSON:
        return "Оценки";
    case DatabaseTypes::ACADEMIC:
        return "Академические";
    case DatabaseTypes::PROJECT:
        return "Проекты";
    case DatabaseTypes::COURSE:
        return "Курсы";
    case DatabaseTypes::DIPLOMA:
        return "Дипломы";
    default:
        return "Неизвестно";
    }
}

bool LogicOperation::sendNotificationWorksTG(TelegramNotifier *tg, const QSet<qint64> &chats, const QVector<Task> &currentTask)
{
    qDebug() << chats.size() << " " << currentTask.size();
    if(!chats.empty() && !currentTask.empty())
    {
        for(const auto& id : chats)
        {
            for(const auto& task : currentTask)
            {
                QString telegramMessage =
                    "🔔 *У вас есть задача:*\n"
                    "──────────────────\n"
                    "📝 *Задача:* " + task.nameT + "\n"
                    "📅 *Срок:* " + task.dateT.toString("dd.MM.yyyy") + "\n"
                    "⏰ *Время:* " + task.timeT.toString("HH:mm") + "\n"
                    "⚡ *Приоритет:* " + task.priorityT + "\n"
                    "──────────────────\n"
                    "📋 *Описание:* " + (task.descT.isEmpty() ? "нет описания" : task.descT);

                tg->sendMessage(id, telegramMessage);

            }
        }
        return true;
    }
    return false;
}



void LogicOperation::ValidFile(QFile &file, const ModeValidator &mode)
{
    try
    {
        Validator::isFileValid(file,mode);
    }catch(const std::logic_error& e)
    {
        QMessageBox::warning(nullptr,"Error",e.what());
        return;
    }
}

bool LogicOperation::DeleteKeyFromRegEdit(const QString &value, const QString &App)
{

    QSettings settings(SerelizationData::ORG_NAME,App);
    QString target = value;

    QStringList allKeys = settings.allKeys();

    for(const auto& c : allKeys)
    {
        QString currentTargetValue = settings.value(c).toString();
         if(target == currentTargetValue)
        {
            settings.remove(c);
            settings.sync();
            return true;
            break;
        }

    }
    return false;
}

QVariant LogicOperation::GetValueRegEditKeys(const QString &key, const QString &app)
{
    QSettings s(SerelizationData::ORG_NAME,app);
    return s.value(key);
}

bool LogicOperation::setDataTableViewEditorForm(QStandardItemModel *model, QList<QHash<QString, QString>> data)
{

    model->clear();

    QStringList headers = data.first().keys();
    model->setHorizontalHeaderLabels(headers);


    for(const auto& row : data)
    {
        QList<QStandardItem*> items;

        for(const auto& header : headers)
        {
            QString v = row.value(header,"");

            items.append(new QStandardItem(v));
        }
        model->appendRow(items);

    }


    return true;
}

