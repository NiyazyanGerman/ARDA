#include "PassportInfo.h"

PassportInfo::PassportInfo(QObject *parent) : QObject(parent) {
    m_issueDate = QDate::currentDate();
}

void PassportInfo::setSeries(const QString &s) { m_series = s.trimmed(); }
void PassportInfo::setNumber(const QString &n) { m_number = n.trimmed(); }
void PassportInfo::setIssuedBy(const QString &i) { m_issuedBy = i; }
void PassportInfo::setIssueDate(const QDate &d) { m_issueDate = d; }
void PassportInfo::setUnitCode(const QString &c) { m_unitCode = c.trimmed(); }

void PassportInfo::setRegion(const QString &r) { m_region = r.trimmed(); }
void PassportInfo::setCity(const QString &c) { m_city = c.trimmed(); }
void PassportInfo::setStreet(const QString &s) { m_street = s.trimmed(); }
void PassportInfo::setHouse(const QString &h) { m_house = h.trimmed(); }
void PassportInfo::setApartment(const QString &a) { m_apartment = a.trimmed(); }

bool PassportInfo::isValid() const {
    return (m_series.length() == 4 && m_number.length() == 6 && !m_city.isEmpty());
}

QString PassportInfo::fullAddress() const {

    QStringList parts;
    if (!m_region.isEmpty()) parts << m_region;
    if (!m_city.isEmpty()) parts << ("г. " + m_city);
    if (!m_street.isEmpty()) parts << ("ул. " + m_street);
    if (!m_house.isEmpty()) parts << ("д. " + m_house);
    if (!m_apartment.isEmpty()) parts << ("кв. " + m_apartment);

    return parts.join(", ");
}

QString PassportInfo::birthPlace() const
{
    return m_birthPlace;
}

void PassportInfo::setBirthPlace(const QString &newBirthPlace)
{
    m_birthPlace = newBirthPlace;
}
