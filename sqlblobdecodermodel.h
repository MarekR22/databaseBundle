#ifndef SQLBLOBDECODERMODEL_H
#define SQLBLOBDECODERMODEL_H

#include <QIdentityProxyModel>

class SqlBlobDecoderModel : public QIdentityProxyModel
{
    Q_OBJECT

    Q_PROPERTY(int imageColumn READ imageColumn WRITE setImageColumn NOTIFY imageColumnChanged)
public:
    using QIdentityProxyModel::QIdentityProxyModel;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int imageColumn() const;
    void setImageColumn(int col);

signals:
    void imageColumnChanged(int col);

private:
    bool isColumnWithImageBlob(const QModelIndex &index) const;
    QPixmap imageFromBlobIn(const QModelIndex &index) const;

private:
    int m_imageColumn = 0;
};

#endif // SQLBLOBDECODERMODEL_H
