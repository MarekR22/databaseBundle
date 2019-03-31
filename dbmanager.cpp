#include "dbmanager.h"
#include "sqlblobdecodermodel.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QFile>

namespace {
const int TableImageColumn = 1;
}

DbManager::DbManager(QObject *parent)
    : QObject(parent)
    , m_tableModel(nullptr)
    , m_budleDecodedModel(nullptr)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("images.db");

    if (!m_db.open())
    {
       qDebug() << "Error: connection with database fail";
    }
    else
    {
       QSqlQuery query(m_db);
       auto ok = query.exec("CREATE TABLE IMAGES (title TEXT, img BLOB);");
       qDebug() << "Database: connection ok. Create table " << ok;
    }
}

QAbstractItemModel* DbManager::imageTableModel()
{
    if (!m_budleDecodedModel && m_db.isOpen())
    {
        m_tableModel = new QSqlTableModel(this, m_db);
        m_tableModel->setTable("IMAGES");
        m_tableModel->select();

        m_budleDecodedModel = new SqlBlobDecoderModel(this);
        m_budleDecodedModel->setSourceModel(m_tableModel);
        m_budleDecodedModel->setImageColumn(TableImageColumn);
    }
    return m_budleDecodedModel;
}

bool DbManager::addImage(const QString &title, const QByteArray &imgData)
{
    if (imgData.isEmpty()) return false;

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO IMAGES (title, img) VALUES (:title, :img);");
    query.bindValue(":title", title);
    query.bindValue(":img", imgData);

    if (!query.exec())
    {
        qDebug() << "add image error:" << query.lastError();
        return false;
    }
    m_tableModel->select();
    return true;
}

bool DbManager::addImage(const QString &title, const QString &fileName)
{
    QFile file{ fileName };
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file: " << fileName;
        return false;
    }
    return addImage(title, file.readAll());
}
