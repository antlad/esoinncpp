#include "nodesmodel.h"

#include <esoinn.h>

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

int NodesModel::rowCount(const QModelIndex &/*parent*/) const
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

void NodesModel::updateModel(const ESOINN &esoinn) const
{
	beginResetModel();
	std::size_t count = esoinn.size();
	m_info.resize(count);

	for (std::size_t i = 0; i < count; ++i)
	{
		m_info[i] = esoinn.nodeInfo(i);
	}
	//esoinn.getLinks()

	endResetModel();
}
