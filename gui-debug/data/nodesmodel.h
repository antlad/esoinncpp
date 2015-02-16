#ifndef NODESMODEL_H
#define NODESMODEL_H

#include <QAbstractItemModel>

#include <vector>
#include <map>

class ESOINN;
class NodeInfo;

class NodesModel
		: public QAbstractItemModel
{
	Q_OBJECT
public:
	explicit NodesModel(QObject *parent = 0);
	virtual QVariant headerData(int section, Qt::Orientation orientation,
								int role = Qt::DisplayRole) const;

	virtual QModelIndex index(int row, int column,
							  const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void updateModel(const ESOINN & esoinn);

private:

	std::map<std::size_t, std::vector<std::size_t> >  m_links;
	std::map<uint32_t, std::vector<NodeInfo *> > m_classes;
	std::vector<NodeInfo> m_apexs;

	std::vector<NodeInfo> m_info;
	std::vector<QImage> m_img;
};

#endif // NODESMODEL_H
