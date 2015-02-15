#include "nodesmodel.h"

#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <esoinn.h>

NodesModel::NodesModel(QObject *parent)
	: QAbstractItemModel(parent)
{
}
const int columns_count = 7;

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
		case 4: return "Distance";
		case 5: return "Image";
		case 6: return "Links";
		}

	}
	else if (orientation == Qt::Vertical && role == Qt::DisplayRole)
	{
		return section;
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
	return columns_count;
}

QVariant NodesModel::data(const QModelIndex &index, int role) const
{
	int r = index.row();
	int c = index.column();
	if (r < 0 || r >= m_info.size() || c < 0 || c >= columns_count)
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		switch(c)
		{
		case 0: return m_info[r].subClass;
		case 1: return m_info[r].realLabel;
		case 2: return m_info[r].density;
		case 3: return m_info[r].winCount;
		case 4: return m_info[r].distance;

		}
	}
	if (role == Qt::DecorationRole )
	{
		if (c == 5)
			return QPixmap::fromImage(m_img[r]);
		else if (c == 6)
		{
			auto it = m_links.find(r);
			if (it == m_links.end())
				return QVariant();

			const std::vector<std::size_t> & ids = (*it).second;

			QImage result(32 * ids.size(), 32, QImage::Format_RGB32);
			if (!ids.empty())
			{
				QPainter painter;
				painter.begin(&result);
				for (int i = 0; i < ids.size(); ++i)
				{
					painter.drawImage(32 * i, 0, m_img[ids[i]]);

				}
				painter.end();
			}
			return QPixmap::fromImage(result);
		}
	}
	if (role == Qt::SizeHintRole && (c == 5 || c == 6))
		return 32;

	return QVariant();
}

class QuickUnion
{


public:
QuickUnion(int N)
{
	count = N;
	id.resize(N);
	sz.resize(N);
	for (int i = 0; i < N; i++)
	{
		id[i] = i;
		sz[i] = 0;
	}
}

bool connected(int p, int q)
{
	return root(p) == root(q);
}

void connect(int p, int q)
{
	int i = root(p);
	int j = root(q);
	if (i == j) return;
	if (sz[i] < sz[j]) { id[i] = j; sz[j] += sz[i]; }
	else { id[j] = i; sz[i] += sz[j]; }
}

private:
	int root(int i)
	{
		while (i != id[i]) {
		i = id[i];
		}
		return i;
	}

	std::vector<int> id;
	std::vector<int> sz;
	int count;
};

void NodesModel::updateModel(const ESOINN &esoinn)
{
	beginResetModel();
	std::size_t count = esoinn.size();
	m_info.resize(count);
	m_img.resize(count);

	for (std::size_t i = 0; i < count; ++i)
	{
		m_info[i] = esoinn.nodeInfo(i);

		QImage img(8, 8, QImage::Format_RGB32);

		for (int x = 0; x < 8; ++x)
		{
			for (int y = 0; y < 8; ++y)
			{
				int val = m_info[i].weights[y * 8 + x] * 255;
				if (val != 0)
				{
					val = val + (val << 8) + (val << 16);
				}
				img.setPixel(x, y, val);
			}
		}
		img = img.scaledToHeight(32);
		img = img.scaledToWidth(32);
		m_img[i] = img;
	}

	m_links = esoinn.getLinks();
//	QuickUnion qunion(count);
//	for (auto it = links.begin(); it != links.end(); ++it)
//	{
//		std::size_t id = (*it).first;
//		const std::vector<std::size_t> & connections = (*it).second;
//		for (std::size_t d : connections)
//		{
//			qunion.connect(id, d);
//		}
//	}

	endResetModel();
}
