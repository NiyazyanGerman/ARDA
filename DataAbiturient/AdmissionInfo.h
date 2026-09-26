#ifndef ADMISSIONINFO_H
#define ADMISSIONINFO_H

#include <QObject>
#include<QDate>
class AdmissionInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString specCode READ specCode WRITE setSpecCode)
    Q_PROPERTY(QString specName READ specName WRITE setSpecName)
    Q_PROPERTY(QString studyForm READ studyForm WRITE setStudyForm)
    Q_PROPERTY(QString studyType READ studyType WRITE setStudyType)
    Q_PROPERTY(QDate applicationDate READ applicationDate WRITE setApplicationDate)
    Q_PROPERTY(QString status READ status WRITE setStatus)
public:
    explicit AdmissionInfo(QObject *parent = nullptr);

    QString specCode() const;
    void setSpecCode(const QString &newSpecCode);

    QString specName() const;
    void setSpecName(const QString &newSpecName);

    QString studyForm() const;
    void setStudyForm(const QString &newStudyForm);

    QString studyType() const;
    void setStudyType(const QString &newStudyType);

    QDate applicationDate() const;
    void setApplicationDate(const QDate &newApplicationDate);

    QString status() const;
    void setStatus(const QString &newStatus);

private:
    QString m_specCode;
    QString m_specName;
    QString m_studyForm;
    QString m_studyType;
    QDate m_applicationDate;
    QString m_status;
};

#endif // ADMISSIONINFO_H
