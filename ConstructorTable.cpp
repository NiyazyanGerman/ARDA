#include "ConstructorTable.h"
#include "ui_ConstructorTable.h"
#include<QTableWidgetItem>
ConstructorTable::ConstructorTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConstructorTable)
{
    ui->setupUi(this);

    // QStringList pgBasicTypes = {
    //     "SMALLINT",
    //     "INTEGER",
    //     "BIGINT",
    //     "SERIAL",
    //     "BIGSERIAL",
    //     "NUMERIC",
    //     "REAL",
    //     "DOUBLE PRECISION",
    //     "CHAR",
    //     "VARCHAR",
    //     "TEXT",
    //     "BOOLEAN",
    //     "DATE",
    //     "TIME",
    //     "TIMESTAMP",
    //     "TIMESTAMPTZ"
    // };




}

ConstructorTable::~ConstructorTable()
{
    delete ui;
}

void ConstructorTable::on_btnAddRow_clicked()
{

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QComboBox *typeCombo = new QComboBox(this);
    typeCombo->addItems({
        "SMALLINT",
        "INTEGER",
        "BIGINT",
        "TEXT",
        "VARCHAR",
        "CHAR",
        "BOOLEAN",
        "DATE",
        "TIME",
        "TIMESTAMP",
        "TIMESTAMPTZ",
        "REAL",
        "DOUBLE PRECISION",
        "NUMERIC",
        "UUID",
        "JSON",
        "JSONB"
    });
    typeCombo->setCurrentText("SMALLINT");
    ui->tableWidget->setCellWidget(row, 1, typeCombo);

    QCheckBox *notNull = new QCheckBox(this);
    notNull->setChecked(true);
    ui->tableWidget->setCellWidget(row, 3, notNull);

    QCheckBox *unique = new QCheckBox(this);
    ui->tableWidget->setCellWidget(row, 4, unique);

    QCheckBox *primary = new QCheckBox(this);
    ui->tableWidget->setCellWidget(row, 5, primary);

    QCheckBox *serial = new QCheckBox(this);
    ui->tableWidget->setCellWidget(row, 7, serial);
}





void ConstructorTable::on_btnRemoveRow_clicked()
{

    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}


void ConstructorTable::on_btnSave_clicked()
{

    tableHandler.clear();
    tableHandler.resize(ui->tableWidget->rowCount());

    for(int row = 0; row < ui->tableWidget->rowCount(); row++)
    {
        for(int col = 0; col < ui->tableWidget->columnCount(); col++)
        {
            QTableWidgetItem* item = ui->tableWidget->item(row, col);
            QWidget* widget = ui->tableWidget->cellWidget(row, col);

            switch(col)
            {
            case 0:
                if(item) tableHandler[row].name = item->text();
                break;

            case 1:
                if (auto combo = qobject_cast<QComboBox*>(widget)) {
                    tableHandler[row].type = combo->currentText();
                }
                break;

            case 2:
                if(item) tableHandler[row].size = item->text().toInt();
                break;

            case 3:
                if (auto not_null = qobject_cast<QCheckBox*>(widget)) {
                    tableHandler[row].not_null = not_null->isChecked();
                }
                break;

            case 4:
                if (auto unique = qobject_cast<QCheckBox*>(widget)) {
                    tableHandler[row].unique = unique->isChecked();
                }
                break;

            case 5:
                if (auto primary = qobject_cast<QCheckBox*>(widget)) {
                    tableHandler[row].primary = primary->isChecked();
                }
                break;

            case 6:
                if(item) tableHandler[row].defaultValue = item->text();
                break;

            case 7:
                if (auto serial = qobject_cast<QCheckBox*>(widget)) {
                    tableHandler[row].serial = serial->isChecked();
                }
                break;
            }
        }
    }

    TableColumn::construct(tableHandler);

    emit cretaeTableReady(tableHandler);

    return;
}

