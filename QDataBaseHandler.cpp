#include "QDataBaseHandler.h"
#include <QSqlError>
#include <QSqlQuery>

QDataBaseHandler::QDataBaseHandler(QObject *parent)
    : QObject{parent}
{
}

bool QDataBaseHandler::create(const DbProperties prop)
{
    propDB = std::make_shared<DbProperties>(std::move(prop));
    return connectToPostgreSQL();
}

bool QDataBaseHandler::createShema(const QString &name)
{
    if (!db.isOpen()) {
        emit getInfo("База данных закрыта. Создать схему нельзя.");

        return false;
    }
    if (!db.isValid()) {
        emit getInfo("База данных невалидна.");
        return false;
    }


    QSqlQuery query(db);
    query.prepare(QString("CREATE SCHEMA IF NOT EXISTS %1").arg(name));

    //query.prepare( QString( "CREATE SCHEMA IF NOT EXTIST %1" ).arg(name) );

    if(!query.exec())
    {
        emit getInfo("Ошибка создания схемы" + query.lastError().text());
        return false;

    }

    emit getInfo(QString("Схема %1 создана (или уже существовала)").arg(name));
    return false;

}

bool QDataBaseHandler::CreateTable(const QString& shema,const QString &tableName, const QList<TableColumn> &columns)
{
    if (!db.isOpen()) {
        emit getInfo("База данных закрыта. Схемы получить нельзя.");

        return {};
    }
    if (!db.isValid()) {
        emit getInfo("База данных невалидна.");
        return {};
    }


    QString sql = QString("CREATE TABLE IF NOT EXISTS %1 (\n").arg(tableName);
    for(auto& c : columns)
    {
        qDebug()<< c.name<< c.type << c.constraints;

    }
    for (int i = 0; i < columns.size(); ++i) {
        const auto& col = columns[i];
        sql += QString("    %1 %2").arg(col.name, col.type);

        if (!col.constraints.isEmpty()) {
            sql += " " + col.constraints;
        }

        if (i < columns.size() - 1) {
            sql += ",";
        }
        sql += "\n";
    }

    sql += ");";

    QSqlQuery query(db);

    query.prepare(sql);

    if (!query.exec(sql)) {
        emit getInfo(QString("Успешно создана таблица в схеме: %1 \n Под названием: %2").arg(shema).arg(tableName));
        return false;
    }
    emit getInfo(QString("Успешно создана таблица в схеме: %1 \n Под названием: %2").arg(shema).arg(tableName));

    return true;

}


bool QDataBaseHandler::connectToPostgreSQL()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QPSQL",CONNECTION_DB_HANDLE);
    }

    db.setHostName(propDB->host);
    db.setUserName(propDB->user);
    db.setPort(propDB->port);
    db.setPassword(propDB->password);
    db.setDatabaseName(propDB->dbName);

    if (db.open()) {
        emit getInfo("Подключились успешно!");
        return true;
    } else {
        emit getInfo("Ошибка подключения: " + db.lastError().text());
        return false;
    }
}

QMap<QString, QStringList> QDataBaseHandler::getSchemes()
{
    if (!db.isOpen()) {
         emit getInfo("База данных закрыта. Схемы получить нельзя.");

        return {};
    }
    if (!db.isValid()) {
        emit getInfo("База данных невалидна.");
        return {};
    }

    QSqlQuery query(db);


    QString sql = R"(
        SELECT schema_name
        FROM information_schema.schemata
        WHERE schema_name NOT IN ('pg_catalog', 'information_schema', 'pg_toast')
          AND schema_name NOT LIKE 'pg_%'
        ORDER BY schema_name;
    )";

    QMap<QString, QStringList> shemes;

    if (query.exec(sql)) {

        while (query.next()) {
            QString schema = query.value(0).toString();
            shemes[schema] = getTables(schema);
        }

    } else {

        emit getInfo("Ошибка получения схем:" + query.lastError().text());

    }

    return shemes;
}

bool QDataBaseHandler::removeShema(const QString &name)
{
    if (!db.isOpen()) {
        emit getInfo("База данных закрыта. Схему удалить нельзя.");

        return {};
    }
    if (!db.isValid()) {
        emit getInfo("База данных невалидна.");
        return {};
    }

    QSqlQuery query(db);

    query.prepare(QString("DROP SCHEMA IF EXISTS %1 CASCADE").arg(name));

    if(!query.exec())
    {
        emit getInfo("Удалить не получилось: " + query.lastError().text());
        return false;
    }
    emit getInfo("Удаление произошло: " + name);
    return true;

}

bool QDataBaseHandler::removeTable(const QString &name)
{
    if (!db.isOpen()) {
        emit getInfo("База данных закрыта. Таблицу удалить нельзя.");

        return {};
    }
    if (!db.isValid()) {
        emit getInfo("База данных невалидна.");
        return {};
    }

    QSqlQuery query(db);

    query.prepare(QString("DROP TABLE IF EXISTS %1 CASCADE").arg(name));

    if(!query.exec())
    {
        emit getInfo("Удалить не получилось: " + query.lastError().text());
        return false;
    }
    emit getInfo("Удаление произошло: " + name);
    return true;
}

QStringList QDataBaseHandler::getTables(const QString& scheme)
{
    QStringList tables;

    if (!db.isOpen()) {
        qDebug() << "База не открыта!";
        return tables;
    }

    QSqlQuery query(db);

    QString sql = R"(
        SELECT t.table_name
        FROM information_schema.tables t
        WHERE t.table_schema = :schema
          AND t.table_type IN ('BASE TABLE', 'VIEW')
        ORDER BY t.table_name;
    )";

    query.prepare(sql);
    query.bindValue(":schema", scheme);

    if (query.exec()) {
        qDebug() << "Таблицы в схеме" << scheme << ":";
        while (query.next()) {
            QString tableName = query.value(0).toString();
            tables << tableName;
        }
    } else {
        qDebug() << "Ошибка получения таблиц:" << query.lastError().text();
    }

    return tables;
}





