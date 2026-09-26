#include "EducationalDocuments.h"


EducationalDocuments::EducationalDocuments(QObject *parent)
 : QObject{parent}
{

}

QString EducationalDocuments::Attestat() const
{
    return m_Attestat;
}

void EducationalDocuments::setAttestat(const QString &newAttestat)
{
    m_Attestat = newAttestat;
}

double EducationalDocuments::averageBall() const
{
    return m_averageBall;
}

void EducationalDocuments::setInstitution(double newAverageBall)
{
    m_averageBall = newAverageBall;
}

double EducationalDocuments::Institution() const
{
    return m_Institution;
}

bool EducationalDocuments::OriginalAttestat() const
{
    return m_OriginalAttestat;
}

void EducationalDocuments::setOriginalAttestat(bool newOriginalAttestat)
{
    m_OriginalAttestat = newOriginalAttestat;
}
