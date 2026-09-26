#include "AdmissionInfo.h"

AdmissionInfo::AdmissionInfo(QObject *parent)
    : QObject{parent}
{}

QString AdmissionInfo::specCode() const
{
    return m_specCode;
}

void AdmissionInfo::setSpecCode(const QString &newSpecCode)
{
    m_specCode = newSpecCode;
}

QString AdmissionInfo::specName() const
{
    return m_specName;
}

void AdmissionInfo::setSpecName(const QString &newSpecName)
{
    m_specName = newSpecName;
}

QString AdmissionInfo::studyForm() const
{
    return m_studyForm;
}

void AdmissionInfo::setStudyForm(const QString &newStudyForm)
{
    m_studyForm = newStudyForm;
}

QString AdmissionInfo::studyType() const
{
    return m_studyType;
}

void AdmissionInfo::setStudyType(const QString &newStudyType)
{
    m_studyType = newStudyType;
}

QDate AdmissionInfo::applicationDate() const
{
    return m_applicationDate;
}

void AdmissionInfo::setApplicationDate(const QDate &newApplicationDate)
{
    m_applicationDate = newApplicationDate;
}

QString AdmissionInfo::status() const
{
    return m_status;
}

void AdmissionInfo::setStatus(const QString &newStatus)
{
    m_status = newStatus;
}
