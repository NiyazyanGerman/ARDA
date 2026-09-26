#ifndef SETCOLUMNTABLEDILOG_H
#define SETCOLUMNTABLEDILOG_H

#include <QDialog>

namespace Ui {
class SetColumnTableDilog;
}

class SetColumnTableDilog : public QDialog
{
    Q_OBJECT

public:
    explicit SetColumnTableDilog(QWidget *parent = nullptr);
    ~SetColumnTableDilog();

private:
    Ui::SetColumnTableDilog *ui;
};

#endif // SETCOLUMNTABLEDILOG_H
