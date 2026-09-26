#include "LessonTableModel.h"
#include<QDate>


LessonTableModel::LessonTableModel(int row, int col, QObject *parent)
: QAbstractTableModel{parent}
    ,rows(row)
    , colums(col)
{

}

QVariant LessonTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    if(index.row() >= rows ||  index.row() < 0 ||index.column() >= colums || index.column() < 0)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return grid_data[index.row()][index.column()];
    }

    return QVariant();

}

bool LessonTableModel::setColumnCount(int col)
{
    if(col == colums || col < 0)
    {
        return false;
    }

    if(col > colums)
    {
        beginInsertColumns(QModelIndex(),colums,col - 1);
        colums = col;
        endInsertColumns();
    }else
    {
        beginRemoveColumns(QModelIndex(), col, colums - 1);
        colums = col;
        endRemoveColumns();
    }

    return true;
}

void LessonTableModel::setHorizontalHeaderLabels(const QStringList &labels)
{
    emit beginResetModel();

    m_headerLabels = labels;

    emit endResetModel();
}

Qt::ItemFlags LessonTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool LessonTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
       if (index.row() >= 0 && index.row() < rows && index.column() >= 0 && index.column() < colums)
        {
            grid_data[index.row()][index.column()] = value.toString();
            emit dataChanged(index,index,{role});
            return true;

       }
    }

    return false;
}

QVariant LessonTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section >= 0 && section < m_headerLabels.size()) {
            return m_headerLabels.at(section);
        }

        return section + 1;
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

bool LessonTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation == Qt::Horizontal && role == Qt::EditRole) {
        if (section >= 0 && section < colums) {

            QDate newDate = value.toDate();

            if (!newDate.isValid()) {
                newDate = QDate::fromString(value.toString(), "dd.MM.yyyy");
            }

            if (newDate.isValid()) {
                emit headerDataChanged(orientation, section, section);
                return true;
            }
        }
    }
}

bool LessonTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{

    if (parent.isValid()) return false;
    if (column < 0 || column > colums) return false;

    beginInsertColumns(parent, column, column + count - 1);

    colums += count;

    for (int i = 0; i < rows; ++i) {
        for (int c = 0; c < count; ++c) {
            grid_data[i].insert(column, "");
        }
    }

    endInsertColumns();
    return true;
}

bool LessonTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid()) return false;
    if (row < 0 || row > rows) return false;

    beginInsertRows(parent,row,row+count - 1);

    rows+= count;

    QVector<QString> emptyRow(colums, "");

    for (int r = 0; r < count; ++r) {
        grid_data.insert(row, emptyRow);
    }

    endInsertRows();

    return true;


}


