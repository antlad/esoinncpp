#ifndef NODESMODEL_H
#define NODESMODEL_H

#include <QAbstractItemModel>

class NodesModel
		: public QAbstractItemModel
{
	Q_OBJECT
public:
	explicit NodesModel(QObject *parent = 0);

	virtual QModelIndex index(int row, int column,
							  const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // NODESMODEL_H
