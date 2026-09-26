#ifndef STUDENTINFO_H
#define STUDENTINFO_H

#include <QObject>

class StudentInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString Name READ Name WRITE setName)
    Q_PROPERTY(QString MiddleName READ MiddleName WRITE setMiddleName)
    Q_PROPERTY(QString Surname  READ Surname  WRITE setSurname )
    Q_PROPERTY(QString SNILS  READ SNILS  WRITE seTSNILS )
    Q_PROPERTY(QString Gender  READ Gender  WRITE setGender )
    Q_PROPERTY(QString NumberPhone  READ NumberPhone  WRITE setNumberPhone )
    Q_PROPERTY(QString citizenship  READ citizenship  WRITE setcitizenship )


public:
    explicit StudentInfo(QObject *parent = nullptr);


    QString Name() const;
    void setName(const QString &newName);

    QString MiddleName() const;
    void setMiddleName(const QString &newMiddleName);

    QString Surname() const;
    void setSurname(const QString &newSurname);

    QString SNILS() const;
    void seTSNILS(const QString &newSNILS);

    QString Gender() const;
    void setGender(const QString &newGender);

    QString NumberPhone() const;
    void setNumberPhone(const QString &newNumberPhone);

    QString citizenship() const;
    void setcitizenship(const QString &newCitizenship);

private:

    QString m_Name;
    QString m_MiddleName;
    QString m_Surname;
    QString m_SNILS;
    QString m_Gender;
    QString m_NumberPhone;
    QString m_citizenship;
};

#endif // STUDENTINFO_H
