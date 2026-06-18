#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>

class Database {
public:
    static Database& instance();
    QSqlDatabase& db();
    bool connect(const QString& host, int port, const QString& dbName,
                 const QString& user, const QString& password);
    bool isConnected() const;

private:
    Database();
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    QSqlDatabase m_db;
};

#endif // DATABASE_H
