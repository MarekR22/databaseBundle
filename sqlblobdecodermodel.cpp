#include "sqlblobdecodermodel.h"
#include <QDebug>
#include <QPixmap>

QVariant SqlBlobDecoderModel::data(const QModelIndex &index, int role) const
{
    if (!isColumnWithImageBlob(index))
        return QIdentityProxyModel::data(index, role);

    if (role == Qt::DecorationRole)
        return imageFromBlobIn(index);
    if (role == Qt::DisplayRole)
        return QString();
    return QIdentityProxyModel::data(index, role);
}

Qt::ItemFlags SqlBlobDecoderModel::flags(const QModelIndex &index) const
{
    if (!isColumnWithImageBlob(index)) {
        return QIdentityProxyModel::flags(index);
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}

int SqlBlobDecoderModel::imageColumn() const
{
    return m_imageColumn;
}

void SqlBlobDecoderModel::setImageColumn(int col)
{
    if (m_imageColumn != col) {
        auto oldCol = m_imageColumn;
        m_imageColumn = col;
        emit imageColumnChanged(col);
        auto lastRow = rowCount() - 1;
        if (lastRow >= 0 && col >= 0 && col < columnCount()) {
            auto roles = QVector<int> { Qt::DisplayRole, Qt::DecorationRole };
            emit dataChanged(index(0, oldCol), index(lastRow, oldCol), roles);
            emit dataChanged(index(0, col), index(lastRow, col), roles);
        }
    }
}

bool SqlBlobDecoderModel::isColumnWithImageBlob(const QModelIndex &index) const
{
    return m_imageColumn == index.column();
}

QPixmap SqlBlobDecoderModel::imageFromBlobIn(const QModelIndex &index) const
{
    auto array = QIdentityProxyModel::data(index, Qt::DisplayRole).toByteArray();
    if (array.isNull()) {
        qDebug() << "No data for pixmap";
        return {};
    }
    QPixmap pixmap;
    if (!pixmap.loadFromData(array,"PNG",Qt::AutoColor)
        && !pixmap.loadFromData(array,"JPG",Qt::AutoColor)) {
        qDebug() << "failed load pixmap";
    }
    return pixmap.scaled(64, 64, Qt::KeepAspectRatio);
}
