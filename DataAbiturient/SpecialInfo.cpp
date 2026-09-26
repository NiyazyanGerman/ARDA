#include "SpecialInfo.h"

SpecialInfo::SpecialInfo(QObject *parent)
    : QObject(parent)
{

}



bool SpecialInfo::hasMedicalCert() const
{
    return m_hasMedicalCert;
}

void SpecialInfo::setHasMedicalCert(bool newHasMedicalCert)
{
    m_hasMedicalCert = newHasMedicalCert;
}

QString SpecialInfo::healthGroup() const
{
    return m_healthGroup;
}

void SpecialInfo::setHealthGroup(const QString &newHealthGroup)
{
    m_healthGroup = newHealthGroup;
}

QString SpecialInfo::physGroup() const
{
    return m_physGroup;
}

void SpecialInfo::setPhysGroup(const QString &newPhysGroup)
{
    m_physGroup = newPhysGroup;
}

QString SpecialInfo::inn() const
{
    return m_inn;
}

void SpecialInfo::setInn(const QString &newInn)
{
    m_inn = newInn;
}

QString SpecialInfo::militaryDoc() const
{
    return m_militaryDoc;
}

void SpecialInfo::setMilitaryDoc(const QString &newMilitaryDoc)
{
    m_militaryDoc = newMilitaryDoc;
}

QString SpecialInfo::benefits() const
{
    return m_benefits;
}

void SpecialInfo::setBenefits(const QString &newBenefits)
{
    m_benefits = newBenefits;
}

QStringList SpecialInfo::achievements() const
{
    return m_achievements;
}

void SpecialInfo::setAchievements(const QStringList &newAchievements)
{
    m_achievements = newAchievements;
}
