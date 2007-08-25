#include "TreeModel.h"
#include <QtDebug>
#include <QColor>
#include <QSize>
#include <iostream>

TreeModel::TreeModel(QStringList& header)
	: m_nHeight(-1)
{
	rootItem = new TreeItem(header);
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

bool TreeModel::hasChildren ( const QModelIndex & parent ) const
{
	TreeItem *parentItem;
	
	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());
	
	return parentItem->childCount() > 0;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->parent();
	
	//std::cout << "parent(), child: " << childItem << "; parent: " << parentItem << "\r\n";

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	
	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

	if (role == Qt::DisplayRole)
		return item->data(index.column());
	if(role == Qt::SizeHintRole && m_nHeight != -1)
		return QSize(50,m_nHeight);
	
	return QVariant();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

Qt::ItemFlags UserTreeModel::flags(const QModelIndex& index) const
{
	if (!index.parent().isValid())
		return Qt::ItemIsEnabled;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant UserTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::DisplayRole || role == Qt::DecorationRole)
		return TreeModel::data(index, role);
	else if(!index.parent().isValid()) // top elem
	{
		if(role == Qt::BackgroundColorRole)
			return qVariantFromValue(QColor(93,95,105));
		else if(role == Qt::TextColorRole)
			return qVariantFromValue(QColor(Qt::white));
		else if(role == Qt::SizeHintRole)
			return QSize(50,25);
	}
	
	return QVariant();
}
QVariant GameTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	
	else if(role == Qt::DecorationRole && index.column() == 1)
	{
		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
		return item->icon();
	}
	else
		return TreeModel::data(index, role);
}

////////////////////////////////////////////////////////////////////////////////////

TreeItem::TreeItem(const QStringList &data, TreeItem *parent)
{
	parentItem = parent;
	itemData = data;
}

TreeItem::TreeItem(QString data, TreeItem *parent)
{
	parentItem = parent;
	itemData = data.split('\n');
	
	//std::cout << "Created item " << (void*)this << "; parent: " << (void*)parentItem << "; text: " << data.toStdString() << "\r\n";
}

TreeItem::TreeItem(TreeItem* parent)
{
	parentItem = parent;
}

TreeItem::~TreeItem()
{
	qDeleteAll(childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
	childItems.append(item);
}
void TreeItem::removeChild(TreeItem *item)
{
	childItems.removeAll(item);
}
void TreeItem::removeAll()
{
	childItems.clear();
}
TreeItem *TreeItem::child(int row)
{
	return childItems.value(row);
}
int TreeItem::childCount() const
{
	return childItems.count();
}
int TreeItem::row() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}
int TreeItem::columnCount() const
{
	return itemData.count();
}
QVariant TreeItem::data(int column) const
{
	return itemData.value(column);
}
TreeItem *TreeItem::parent()
{
	return parentItem;
}

