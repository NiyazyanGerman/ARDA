#ifndef QDATABASEHANDLER_H
#define QDATABASEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#define CONNECTION_DB_HANDLE "akuma"

struct TableRowState {
    int row;
    int column;
    QVariant value;

};
struct TableColumn
{
    QString name;
    QString type;
    QString constraints;
    int size = false;
    bool not_null = false;
    bool unique = false;
    bool primary = false;
    bool serial = false;
    QString defaultValue;

    static void construct(QList<TableColumn>& table)
    {
        for(auto& col : table)
        {
            if (col.size > 0 && (col.type == "VARCHAR" || col.type == "CHAR"))
            {
                col.type += "(" + QString::number(col.size) + ")";
            }

            col.constraints.clear();

            if (col.not_null)
                col.constraints += " NOT NULL";

            if (col.unique)
                col.constraints += " UNIQUE";

            if (col.primary)
                col.constraints += " PRIMARY KEY";

            if (col.serial)
                col.constraints += " SERIAL";

            col.constraints = col.constraints.trimmed();
        }
    }

};
struct DbProperties
{
    DbProperties() {}

    QString host = "localhost",user = "postgres",dbName = "postgres";
    QString password = "";
    int port = 5432;

    bool isEmpty() const
    {
        return !host.isEmpty() || !user.isEmpty() || !dbName.isEmpty() || !password.isEmpty();
    }
};

class QDataBaseHandler : public QObject
{
    Q_OBJECT
public:
    explicit QDataBaseHandler(QObject *parent = nullptr);

    bool create(const DbProperties prop);
    bool createShema(const QString& name);
    bool CreateTable(const QString &shema, const QString& tableName, const QList<TableColumn>& columns);
    QMap<QString, QStringList> getSchemes();

    bool removeShema(const QString& name);
    bool removeTable(const QString& name);

signals:
    void getInfo(const QString& log);

private:
    bool connectToPostgreSQL();
    QStringList getTables(const QString& scheme);

    std::shared_ptr<DbProperties> propDB;
    QSqlDatabase db;
};

#endif // QDATABASEHANDLER_H
