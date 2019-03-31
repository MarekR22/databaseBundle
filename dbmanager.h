#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class QAbstractItemModel;
class QSqlTableModel;
class SqlBlobDecoderModel;

class DbManager : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(QObject *parent = nullptr);

    QAbstractItemModel* imageTableModel();

public slots:
    bool addImage(const QString& title, const QByteArray& imgData);
    bool addImage(const QString& title, const QString& fileName);

private:
    QSqlDatabase m_db;
    QSqlTableModel* m_tableModel;
    SqlBlobDecoderModel* m_budleDecodedModel;
};

#endif // DBMANAGER_H
