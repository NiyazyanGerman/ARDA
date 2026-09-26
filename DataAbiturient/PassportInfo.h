#ifndef PASSPORTINFO_H
#define PASSPORTINFO_H

#include <QObject>
#include <QString>
#include <QDate>

class PassportInfo : public QObject
{
    Q_OBJECT
    // Данные документа
    Q_PROPERTY(QString series READ series WRITE setSeries)
    Q_PROPERTY(QString number READ number WRITE setNumber)
    Q_PROPERTY(QString issuedBy READ issuedBy WRITE setIssuedBy)
    Q_PROPERTY(QDate issueDate READ issueDate WRITE setIssueDate)
    Q_PROPERTY(QString unitCode READ unitCode WRITE setUnitCode)
    Q_PROPERTY(QString birthPlace READ birthPlace WRITE setBirthPlace)

    // Адрес регистрации
    Q_PROPERTY(QString region READ region WRITE setRegion)
    Q_PROPERTY(QString city READ city WRITE setCity)
    Q_PROPERTY(QString street READ street WRITE setStreet)
    Q_PROPERTY(QString house READ house WRITE setHouse)
    Q_PROPERTY(QString apartment READ apartment WRITE setApartment)

public:
    explicit PassportInfo(QObject *parent = nullptr);

    QString series() const { return m_series; }
    QString number() const { return m_number; }
    QString issuedBy() const { return m_issuedBy; }
    QDate issueDate() const { return m_issueDate; }
    QString unitCode() const { return m_unitCode; }

    QString region() const { return m_region; }
    QString city() const { return m_city; }
    QString street() const { return m_street; }
    QString house() const { return m_house; }
    QString apartment() const { return m_apartment; }

    void setSeries(const QString &s);
    void setNumber(const QString &n);
    void setIssuedBy(const QString &i);
    void setIssueDate(const QDate &d);
    void setUnitCode(const QString &c);

    void setRegion(const QString &r);
    void setCity(const QString &c);
    void setStreet(const QString &s);
    void setHouse(const QString &h);
    void setApartment(const QString &a);


    bool isValid() const;
    QString fullAddress() const;

    QString birthPlace() const;
    void setBirthPlace(const QString &newBirthPlace);

private:
    QString m_series;
    QString m_number;
    QString m_issuedBy;
    QDate m_issueDate;
    QString m_unitCode;
    QString m_birthPlace;

    QString m_region;
    QString m_city;
    QString m_street;
    QString m_house;
    QString m_apartment;
};

#endif // PASSPORTINFO_H
