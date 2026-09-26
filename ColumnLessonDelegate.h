#ifndef COLUMNLESSONDELEGATE_H
#define COLUMNLESSONDELEGATE_H

#include <QStyledItemDelegate>
#include<QLineEdit>
#include<QObject>
#include<QWidget>
#include<QVBoxLayout>



class ColumnLessonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ColumnLessonDelegate(QObject * parent = nullptr);


    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // COLUMNLESSONDELEGATE_H
