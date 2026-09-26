#pragma once

#include <QObject>
#include <memory>
#include <vector>
#include <QTableWidget>
#include <QtConcurrent/QtConcurrent>
#include "JsonManagerIO/ManagerJsonLesson.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include "UtilsAutoSaver.h"

template<typename T>
class SerelizerJsonManagerLessonModule : public QObject
{
public:
    explicit SerelizerJsonManagerLessonModule(QObject *parent = nullptr)
        : serelization(std::make_unique<ManagerJsonLesson>())
        , utils(std::make_unique<UtilsAutoSaver>())
        , fileSave(QString("temp%1.json").arg("1"))
        , QObject(parent)
    {

    }

    bool  LoadOldSerelizationDataFile()
    {

        QFuture<bool> future= QtConcurrent::run([this](){
            lessonDataOld = std::move(serelization->LoadAdapter(fileSave));
            return lessonDataOld.empty();
        });

        bool succes = future.result();
        return succes;
    }

    bool compareRows(std::vector<std::unique_ptr<T>> &newData)
    {

        qDebug() << "size curent " << newData.size() << " old size " << lessonDataOld.size();
        if (newData.size() != lessonDataOld.size()) {
            qDebug() << "РАЗНЫЙ РАЗМЕР -> сохраняем";
            return true;
        }

        if (lessonDataOld.empty()) {
            qDebug() << "lessonDataOld пуст -> сохраняем";
            return true;
        }

        for (size_t i = 0; i < newData.size(); ++i)
        {
            if(!newData.at(i) || !lessonDataOld.at(i))
            {
                qDebug() << "Nullptr в индексе" << i << "-> сохраняем";
                return true;
            }

            if (*newData[i] != *lessonDataOld[i]) {
                qDebug() << "ДАННЫЕ ОТЛИЧАЮТСЯ в индексе" << i << "-> сохраняем";
                return true;
            }
        }

        qDebug() << "ДАННЫЕ ИДЕНТИЧНЫ -> НЕ сохраняем";
        return false;
    }

    bool updateAdater(std::vector<std::unique_ptr<T>> &newData)
    {
        lessonDataOld = std::move(newData);
        qDebug() << lessonDataOld.size()<< "current";
        return true;
    }

    bool load(QTableWidget* table)
    {

        //LoadOldSerelizationDataFile();
        table->setRowCount(lessonDataOld.size());
        table->setColumnCount(lessonDataOld.at(0).get()->columnCount);

        if (typeid(*lessonDataOld.at(0)) == typeid(LessonAdapter)) {
                QStringList headers = {"ID", "ФИО", "Группа", "Название Урока"};
                for(const auto& [date,mark] : lessonDataOld.at(0)->daysInfo.toStdMap())
                {
                    headers += date;
                }



            table->setHorizontalHeaderLabels(headers);

            for(int row = 0; row < lessonDataOld.size() ; row++ )
            {
                table->setItem(row,0,new QTableWidgetItem(lessonDataOld[row]->id));
                table->setItem(row,1,new QTableWidgetItem(lessonDataOld[row]->fio));
                table->setItem(row,2,new QTableWidgetItem(lessonDataOld[row]->group));
                table->setItem(row,3,new QTableWidgetItem(lessonDataOld[row]->lessonName));


            }

            for(int day = 0; day < lessonDataOld.size(); day++)
            {
                int col = 4;
                auto key = lessonDataOld.at(day)->daysInfo.begin();
                QString value = key.value();
                table->setItem(day,col,new QTableWidgetItem(value));
                col++;
            }
        }

        return true;
    }

    std::vector<std::unique_ptr<T>> SerelizerJSON(const QTableWidget* table)
    {

        std::vector<std::unique_ptr<T>> result;

        for (int row = 0; row < table->rowCount(); ++row) {
            auto adapter = std::make_unique<T>();

            fillAdapterFromRow(adapter.get(), table, row);

            adapter->loadFromTableRow(table, row); // skip LessonAdapter type

            result.push_back(std::move(adapter));
        }

        qDebug() << "result" << result.size() << " result Type" << static_cast<int>(result[0]->getType());
        return result;

    }

    void fillAdapterFromRow(T *adapter, const QTableWidget *table, int row)
    {

        auto id = table->item(row,0);
        auto fio = table->item(row,1);
        auto group = table->item(row,2);

        if (id) adapter->id = id->text();
        if (fio) adapter->fio = fio->text();
        if (group) adapter->group = group->text();

        if (typeid(*adapter) == typeid(LessonAdapter))
        {
            auto lessonName = table->item(row,3);
            if (lessonName) adapter->lessonName = lessonName->text();

            for (int col = 4; col < table->columnCount(); ++col) {
                auto headerItem = table->horizontalHeaderItem(col);
                auto cellItem = table->item(row, col);
                if (headerItem && cellItem) {
                    adapter->daysInfo[headerItem->text()] = cellItem->text();
                }
            }

            qDebug() << "Fill";
        }


    }

public slots:

    bool run(const QTableWidget *table)
    {
        if (!table) {
            qDebug() << "Ошибка: table = nullptr";
            return false;
        }

        auto currentData = SerelizerJSON(table);

        auto dataForSave = utils->deepCopy(currentData);

        qDebug() << "copy Type " << static_cast<int>(dataForSave[0]->getType());

        if (!compareRows(currentData)) {
            qDebug() << "Нет изменений, сохранение пропущено";
            return false;
        }

        QFutureWatcher<bool>* watcher = new QFutureWatcher<bool>(this);

        connect(watcher, &QFutureWatcher<bool>::finished, this,[this, watcher, currentData = std::move(currentData)]() mutable {
            bool success = watcher->result();
            if (success) {
                updateAdater(currentData);
                qDebug() << "Сохранение успешно завершено";
            } else {
                qDebug() << "Ошибка сохранения";
            }
            watcher->deleteLater();
        });

        QFuture<bool> future = QtConcurrent::run([this, Copy = std::move(dataForSave)]() mutable {
            bool result = serelization->ParserAdapter(Copy, fileSave);
            if (result) {
                emit serelization->WriteSucceful();
            }
            return result;
        });

        watcher->setFuture(future);
        return true;

    }

private:
    std::unique_ptr<ManagerJsonLesson> serelization;
    std::unique_ptr<UtilsAutoSaver> utils;
    std::vector<std::unique_ptr<T>> lessonDataOld;


    QFile fileSave;
};
