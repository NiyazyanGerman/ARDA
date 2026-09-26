#ifndef HTTPSERVERFORM_H
#define HTTPSERVERFORM_H

#include <QWidget>
#include"ShareManagerHTTP.h"

namespace Ui {
class HTTPserverForm;
}

class HTTPserverForm : public QWidget
{
    Q_OBJECT

public:
    explicit HTTPserverForm(QWidget *parent = nullptr);
    ~HTTPserverForm();

private:
    Ui::HTTPserverForm *ui;


};

#endif // HTTPSERVERFORM_H
