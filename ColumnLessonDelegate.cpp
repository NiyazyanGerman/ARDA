#include "ColumnLessonDelegate.h"


ColumnLessonDelegate::ColumnLessonDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *ColumnLessonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column()) {
    case 0:
        return QStyledItemDelegate::createEditor(parent, option, index);
        break;
    case 1:
        return QStyledItemDelegate::createEditor(parent, option, index);
        break;
    case 2:
        return QStyledItemDelegate::createEditor(parent, option, index);
        break;
    case 3:
        return QStyledItemDelegate::createEditor(parent, option, index);
        break;
    default:
        QWidget *container = new QWidget(parent);
        QHBoxLayout *layout = new QHBoxLayout(container);

        layout->setContentsMargins(2, 2, 2, 2);
        layout->setSpacing(4);

        QLineEdit *edit1 = new QLineEdit(container);
        QLineEdit *edit2 = new QLineEdit(container);

        edit1->setObjectName("edit1");
        edit2->setObjectName("edit2");

        layout->addWidget(edit1);
        layout->addWidget(edit2);
        container->setLayout(layout);

        container->setFocusProxy(edit1);

        return container;
        break;
    }

}

void ColumnLessonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() > 3) {
        QLineEdit *edit1 = editor->findChild<QLineEdit*>("edit1");
        QLineEdit *edit2 = editor->findChild<QLineEdit*>("edit2");

        QString text = index.model()->data(index, Qt::EditRole).toString();
        QStringList parts = text.split(" | ");

        if (edit1) edit1->setText(parts.value(0));
        if (edit2) edit2->setText(parts.value(1));
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}
void ColumnLessonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if (index.column() > 3) {
        QLineEdit *edit1 = editor->findChild<QLineEdit*>("edit1");
        QLineEdit *edit2 = editor->findChild<QLineEdit*>("edit2");

        QString text = edit1->text() + " | " + edit2->text();
        model->setData(index, text, Qt::EditRole);
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void ColumnLessonDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}

