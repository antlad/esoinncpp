#include "nodesmodel.h"

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <esoinn.h>

NodesModel::NodesModel(QObject *parent)
	: QAbstractItemModel(parent)
{
}
const int columns_count = 8;

QVariant NodesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0:
			return "Sub class";
		case 1:
			return "Real label";
		case 2:
			return "Density";
		case 3:
			return "Win count";
		case 4:
			return "Distance";
		case 5:
			return "Image";
		case 6:
			return "Links";
		case 7:
			return "Links ids";
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
		case 0:
			return m_info[r].subClass;
		case 1:
			return m_info[r].realLabel;
		case 2:
			return m_info[r].density;
		case 3:
			return m_info[r].winCount;
		case 4:
			return m_info[r].distance;
		case 7:
		{

			auto it = m_links.find(r);
			if (it == m_links.end())
				return QVariant();

			QString result;
			const std::vector<std::size_t> & ids = (*it).second;

			for (auto id : ids)
			{
				result.append(QString::number(id)).append(' ');
			}
			return result;
		}

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

void NodesModel::updateModel(const ESOINN &esoinn)
{
	beginResetModel();
	std::size_t count = esoinn.size();
	m_info.resize(count);
	m_img.resize(count);

	for (std::size_t i = 0; i < count; ++i)
	{
		const NodeInfo & info = esoinn.nodeInfo(i);
		m_info[i] = info;
		m_classes[info.subClass].push_back(&m_info[i]);

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

	endResetModel();
}
