#ifndef ABITURIENTFORM_H
#define ABITURIENTFORM_H

#include <QWidget>

namespace Ui {
class AbiturientForm;
}

class AbiturientForm : public QWidget
{
    Q_OBJECT

public:
    explicit AbiturientForm(QWidget *parent = nullptr);
    ~AbiturientForm();

private:
    Ui::AbiturientForm *ui;
};

#endif // ABITURIENTFORM_H
