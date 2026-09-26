#ifndef EDUCATIONALDOCUMENTS_H
#define EDUCATIONALDOCUMENTS_H

#include<QObject>

class EducationalDocuments : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString Attestat READ Attestat WRITE setAttestat)
    Q_PROPERTY(double averageBall READ averageBall WRITE setInstitution )
    Q_PROPERTY(double Institution  READ Institution  WRITE setInstitution )
    Q_PROPERTY(bool OriginalAttestat READ OriginalAttestat WRITE setOriginalAttestat)

public:
    explicit EducationalDocuments(QObject *parent = nullptr);

    QString Attestat() const;
    void setAttestat(const QString &newAttestat);

    double averageBall() const;
    double Institution() const;

    void setInstitution(double newAverageBall);


    bool OriginalAttestat() const;
    void setOriginalAttestat(bool newOriginalAttestat);

private:
    QString m_Attestat;
    double m_averageBall;
    double m_Institution;
    bool m_OriginalAttestat;
};

#endif // EDUCATIONALDOCUMENTS_H
