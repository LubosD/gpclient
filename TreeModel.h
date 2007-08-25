#ifndef TREEMODEL_H
#define TREEMODEL_H
#include <QAbstractListModel>
#include <QStringList>
#include <QTreeView>

class TreeItem;

class TreeModel : public QAbstractListModel
{
public:
	TreeModel(QStringList& header);
	~TreeModel();
	void setRowHeight(int height) { m_nHeight = height; }
	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex &index) const;
	//Qt::ItemFlags flags(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	QVariant data(const QModelIndex &index, int role) const;
	bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const;
	void doReset() { emit reset(); }
	//Qt::ItemFlags flags(const QModelIndex &index) const;
	
	TreeItem* rootItem;
private:
	int m_nHeight;
};
class UserTreeModel : public TreeModel
{
public:
	UserTreeModel(QStringList& header) : TreeModel(header) {}
	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
};
class GameTreeModel : public TreeModel
{
public:
	GameTreeModel(QStringList& header) : TreeModel(header) {}
	QVariant data(const QModelIndex &index, int role) const;
};
/*class ServerTreeModel : public TreeModel
{
public:
};*/

class TreeItem
{
	public:
		TreeItem(const QStringList &data, TreeItem *parent = 0);
		TreeItem(QString data, TreeItem *parent = 0);
		TreeItem(TreeItem* parent = 0);
		virtual ~TreeItem();

		void appendChild(TreeItem *child);
		void removeChild(TreeItem *item);
		void removeAll();
		void setData(QStringList& data) { itemData = data; }
		void setIcon(QIcon icon) { m_icon = icon; }

		QIcon icon() const { return m_icon; }
		TreeItem *child(int row);
		int childCount() const;
		int columnCount() const;
		QVariant data(int column) const;
		int row() const;
		TreeItem *parent();
	private:
		QList<TreeItem*> childItems;
		QIcon m_icon;
		QStringList itemData;
		TreeItem *parentItem;
};

class GameTreeItem : public TreeItem
{
public:
	GameTreeItem(TreeItem* parent = 0)
	: TreeItem(parent)
	{
	}
	
	bool m_bPassworded;
};

#endif
