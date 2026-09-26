#ifndef ADDHINTSFORM_H
#define ADDHINTSFORM_H

#include <QWidget>
#include"scheduledataservice.h"
#include"dataschedule.h"
#include"ScheduleJsonParser.h"
namespace Ui {
class AddHintsForm;
}

class AddHintsForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddHintsForm(QWidget *parent = nullptr);
    ~AddHintsForm();

private:
    Ui::AddHintsForm *ui;
    std::unique_ptr<ScheduleDataService> serviceScheudle;
    std::unique_ptr<DataSchedule> scheudle;
    std::unique_ptr<ScheduleJsonParser> jsonParser;

    void initComboBox();
};

#endif // ADDHINTSFORM_H
