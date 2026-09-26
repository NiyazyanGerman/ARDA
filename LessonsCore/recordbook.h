#ifndef RECORDBOOK_H
#define RECORDBOOK_H

#include <QWidget>
#include"../IOCore/IODataHandler.h"
#include"../LogicOperation.h"
#include<QTableWidgetItem>
#include"../Serializer.h"
#include"../IOCore/iodatabasehandler.h"
#include"DataWritterStudent.h"
#include"ITablesManager.h"
#include"../AutoSaverLesson/SerelizerJsonManagerLessonModule.h"

class MainWindow;

namespace Ui {
class RecordBook;
}

class RecordBook : public ITablesManager
{
    Q_OBJECT

public:
    explicit RecordBook(MainWindow* main, ITablesManager *parent = nullptr);
    ~RecordBook();

private:


    Ui::RecordBook *ui;
    MainWindow* backWindow;
    ImportSaveData* obj;
    LogicOperation* ihs;
    std::unique_ptr<IODataBaseHandler> dbHandler;
    FileManager* fileManager;
    std::unique_ptr<SerializerData> serilizer;
    std::unique_ptr<DataWritterStudent> dataWriterInfo;
    std::unique_ptr<SerelizerJsonManagerLessonModule<RecordAdapter>> ser;

    int countPair = 0;

    QAction* SaveRecordPDF;
    QAction* SaveRecordJSON;
    QAction* uploadReadyData;
    QAction* SaveData;
    QAction* Quit;
    QAction* Clear;
    QAction* Print;


    void setupConnections();
    const int checkboxColumn = 8;
    const int resultColumn = 9;


private slots:
    void Back();
    void addRow();
    void DeleteRow();
    void ClearTable();
    void PrintData();
    void FindName();
    void AutoTest(QTableWidgetItem* item);

public slots:
    QString SetHeaderDocument() override;
    QString SetNameDocument() override;
    QString setFormat(const QJsonObject &obj) override;


};

#endif // RECORDBOOK_H
