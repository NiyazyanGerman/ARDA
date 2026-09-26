#ifndef CURRENTSESSION_H
#define CURRENTSESSION_H

#include <QWidget>
#include"ActiveSession.h"
namespace Ui {
class CurrentSession;
}

class CurrentSession : public QWidget
{
    Q_OBJECT

public:
    explicit CurrentSession(QWidget *parent = nullptr);
    ~CurrentSession();

    void initUI(const ActiveSession* session);

private:
    Ui::CurrentSession *ui;
};

#endif // CURRENTSESSION_H
