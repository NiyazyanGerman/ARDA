#include "StudentInfo.h"

StudentInfo::StudentInfo(QObject *parent)
    : QObject{parent}
{}

QString StudentInfo::Name() const
{
    return m_Name;
}

void StudentInfo::setName(const QString &newName)
{
    m_Name = newName;
}

QString StudentInfo::MiddleName() const
{
    return m_MiddleName;
}

void StudentInfo::setMiddleName(const QString &newMiddleName)
{
    m_MiddleName = newMiddleName;
}

QString StudentInfo::Surname() const
{
    return m_Surname;
}

void StudentInfo::setSurname(const QString &newSurname)
{
    m_Surname = newSurname;
}

QString StudentInfo::SNILS() const
{
    return m_SNILS;
}

void StudentInfo::seTSNILS(const QString &newSNILS)
{
    m_SNILS = newSNILS;
}

QString StudentInfo::Gender() const
{
    return m_Gender;
}

void StudentInfo::setGender(const QString &newGender)
{
    m_Gender = newGender;
}

QString StudentInfo::NumberPhone() const
{
    return m_NumberPhone;
}

void StudentInfo::setNumberPhone(const QString &newNumberPhone)
{
    m_NumberPhone = newNumberPhone;
}

QString StudentInfo::citizenship() const
{
    return m_citizenship;
}

void StudentInfo::setcitizenship(const QString &newCitizenship)
{
    m_citizenship = newCitizenship;
}
