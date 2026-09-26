#ifndef TEXPANEL_H
#define TEXPANEL_H

#include <QWidget>
#include "../QDataBaseHandler.h"
#include <QSqlTableModel>
#include"../ConstructorTable.h"

namespace Ui {
class TexPanel;
}

class TexPanel : public QWidget
{
    Q_OBJECT

public:
    explicit TexPanel(QWidget *parent = nullptr);
    ~TexPanel();

private slots:

    void on_btnConnect_clicked();
    void viewDB();
    void on_btnLoadCurrentTable_clicked();
    void on_btnNewScheme_clicked();
    void on_btnRefersh_clicked();
    void on_btnAddRow_clicked();
    void on_btnRemoveRow_clicked();
    void on_btnSave_clicked();
    // void on_btnUndo_clicked();
    void on_btnBackUp_clicked();

    void on_btnAddTable_clicked();

private:
  void onTreeContextMenu(const QPoint& pos);

    Ui::TexPanel *ui;
    std::unique_ptr<QDataBaseHandler> handlerDb;
    DbProperties prop;
    QMap<QString, QStringList> shemes;
    QSqlTableModel* model;
    QSqlDatabase m_db;

    ConstructorTable* table;
};

#endif // TEXPANEL_H
