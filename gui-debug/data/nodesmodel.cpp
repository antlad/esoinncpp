#include "nodesmodel.h"

#include <esoinn.h>

NodesModel::NodesModel(QObject *parent)
	: QAbstractItemModel(parent)
{

}

QVariant NodesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0: return "Sub class";
		case 1: return "Real label";
		case 2: return "Density";
		case 3: return "Win count";
		case 4: return "Image";
		}

	}
	return QVariant();
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
	return m_info.size();
}

int NodesModel::columnCount(const QModelIndex &parent) const
{
	return 5;
}

QVariant NodesModel::data(const QModelIndex &index, int role) const
{
	int r = index.row();
	int c = index.column();
	if (r < 0 || r >= m_info.size() || c < 0 || c >= 5)
		return QVariant();
	if (role == Qt::DisplayRole)
	{
		switch(c)
		{
			case 0: return m_info[r].subClass;
			case 1: return m_info[r].realLabel;
			case 2: return m_info[r].density;
			case 3: return m_info[r].winCount;
			case 4: return QVariant();/*m_info[r].subClass*/;
		}
	}
	return QVariant();
}

void NodesModel::updateModel(const ESOINN &esoinn)
{
	beginResetModel();
	std::size_t count = esoinn.size();
	m_info.resize(count);

	for (std::size_t i = 0; i < count; ++i)
	{
		m_info[i] = esoinn.nodeInfo(i);
	}

	endResetModel();
}
