#include "database.h"
#include <QDebug>
#include <QSqlError>

Database::Database() {}

Database::~Database() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

Database& Database::instance() {
    static Database inst;
    return inst;
}

QSqlDatabase& Database::db() {
    return m_db;
}

bool Database::connect(const QString& host, int port, const QString& dbName,
                       const QString& user, const QString& password) {
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(host);
    m_db.setPort(port);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);

    if (!m_db.open()) {
        qDebug() << "Database connection error:" << m_db.lastError().text();
        return false;
    }
    return true;
}

bool Database::isConnected() const {
    return m_db.isOpen();
}
