#ifndef SPECIALINFO_H
#define SPECIALINFO_H

#include <QObject>
#include <QString>
#include <QStringList>

class SpecialInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hasMedicalCert READ hasMedicalCert WRITE setHasMedicalCert)
    Q_PROPERTY(QString healthGroup READ healthGroup WRITE setHealthGroup)
    Q_PROPERTY(QString physGroup READ physGroup WRITE setPhysGroup)
    Q_PROPERTY(QString inn READ inn WRITE setInn)
    Q_PROPERTY(QString militaryDoc READ militaryDoc WRITE setMilitaryDoc)
    Q_PROPERTY(QString benefits READ benefits WRITE setBenefits)
    Q_PROPERTY(QStringList achievements READ achievements WRITE setAchievements)

public:
    explicit SpecialInfo(QObject *parent = nullptr);


    bool hasMedicalCert() const;
    void setHasMedicalCert(bool newHasMedicalCert);

    QString healthGroup() const;
    void setHealthGroup(const QString &newHealthGroup);

    QString physGroup() const;
    void setPhysGroup(const QString &newPhysGroup);

    QString inn() const;
    void setInn(const QString &newInn);

    QString militaryDoc() const;
    void setMilitaryDoc(const QString &newMilitaryDoc);

    QString benefits() const;
    void setBenefits(const QString &newBenefits);

    QStringList achievements() const;
    void setAchievements(const QStringList &newAchievements);

private:

    bool m_hasMedicalCert;
    QString m_healthGroup;
    QString m_physGroup;
    QString m_inn;
    QString m_militaryDoc;
    QString m_benefits;
    QStringList m_achievements;
};

#endif // SPECIALINFO_H
