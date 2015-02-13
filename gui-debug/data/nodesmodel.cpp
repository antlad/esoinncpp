#include "nodesmodel.h"

NodesModel::NodesModel(QObject *parent)
	: QAbstractItemModel(parent)
{

}

QModelIndex NodesModel::index(int row, int column, const QModelIndex &parent) const
{
	return createIndex(row, column);
}

QModelIndex NodesModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}

int NodesModel::rowCount(const QModelIndex &parent) const
{
	return 0;
}

int NodesModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant NodesModel::data(const QModelIndex &index, int role) const
{
	return QVariant();
}
