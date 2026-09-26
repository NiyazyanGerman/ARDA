#ifndef ACCOUNTINGASSESSMENTS_H
#define ACCOUNTINGASSESSMENTS_H

#include <QWidget>
#include<QSettings>
#include"../IOCore/IODataHandler.h"
#include"../Serializer.h"
#include"../FileChooicer.h"
#include<QComboBox>
#include"../IOCore/iodatabasehandler.h"
#include"../tablemanager.h"
#include"ITablesManager.h"
#include"DataWritterStudent.h"
#include<QTimer>
#include"../AutoSaverLesson/SerelizerJsonManagerLessonModule.h"
#include"../AutoSaverLesson/AdapterTables.h"

#include"../LessonTableModel.h"
#include"../ColumnLessonDelegate.h"
#include<QStandardItemModel>
#include"../ApiHandler.h"
#include"../ActiveSession.h"
#define DEFAULT_TABLES 4

class academicrecordwindow;
class MainWindow;

namespace Ui {
class accountingassessments;
}


class accountingassessments : public QWidget
{
    //Q_OBJECT

public:
    explicit accountingassessments(MainWindow* mainWindow, QWidget *parent = nullptr);

    ~accountingassessments();

    bool initDatabase();
    void onAddRecord();
    void onSaveToDatabase();
    void editColumnName();

private:

    Ui::accountingassessments *ui;
    std::unique_ptr<SerelizerJsonManagerLessonModule<LessonAdapter>> serelizationJson;

    MainWindow* backWindow;
    FileManager* fileManager;
    QTimer* timer;
    LessonTableModel* m_model;
    std::unique_ptr<ApiHandler> handlerRequest;
public slots:
   // bool addDay()
   // QString SetHeaderDocument() override;
   // QString SetNameDocument() override;
   // QString setFormat(const QJsonObject &obj) override;


private slots:
    void on_btnAddDay_clicked();
    void on_btnSave_clicked();

    void on_btnGetGroup_clicked();

};

#endif // ACCOUNTINGASSESSMENTS_H
