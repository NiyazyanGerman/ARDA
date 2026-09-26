#ifndef CONSTRUCTORTABLE_H
#define CONSTRUCTORTABLE_H

#include <QWidget>
#include<QComboBox>
#include"QDataBaseHandler.h"
#include<QListWidget>
#include<QCheckBox>
namespace Ui {
class ConstructorTable;
}

class ConstructorTable : public QWidget
{
    Q_OBJECT

public:
    explicit ConstructorTable(QWidget *parent = nullptr);
    ~ConstructorTable();

private slots:
    void on_btnAddRow_clicked();
    void on_btnRemoveRow_clicked();
    void on_btnSave_clicked();

signals:
    void cretaeTableReady(const QList<TableColumn>& handlers) ;

private:
    Ui::ConstructorTable *ui;

    QStringList ConstraintsList;
    QList<TableColumn> tableHandler;


};

#endif // CONSTRUCTORTABLE_H
