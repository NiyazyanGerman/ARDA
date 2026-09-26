#ifndef LESSONTABLEMODEL_H
#define LESSONTABLEMODEL_H

#include <QAbstractTableModel>
#include<QHash>
class LessonTableModel : public QAbstractTableModel
{


public:
    explicit LessonTableModel(int row,int col,QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    int rowCount(const QModelIndex &parent) const override
    {
         Q_UNUSED(parent);
        if(parent.isValid())
        {
            return 0;
        }
        return rows;
    }

    int columnCount(const QModelIndex &parent) const override{
         Q_UNUSED(parent);
        if(parent.isValid())
        {
            return 0;
        }
        return colums;
    }

    bool setColumnCount(int col);
    void setHorizontalHeaderLabels(const QStringList &labels);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) override;


    bool insertColumns(int column, int count, const QModelIndex &parent) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
   // bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
   // bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;


private:
    int rows;
    int colums;
    QVector<QVector<QString>> grid_data;
 QStringList m_headerLabels;
};

#endif // LESSONTABLEMODEL_H
