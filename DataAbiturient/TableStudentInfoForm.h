#ifndef TABLESTUDENTINFOFORM_H
#define TABLESTUDENTINFOFORM_H

#include <QWidget>

namespace Ui {
class TableStudentInfoForm;
}

class TableStudentInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit TableStudentInfoForm(QWidget *parent = nullptr);
    ~TableStudentInfoForm();

private:
    Ui::TableStudentInfoForm *ui;
};

#endif // TABLESTUDENTINFOFORM_H
