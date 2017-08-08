#include "acSelectionModel.h"

class acSelectionDataItem : public QObject
{
public:
	acSelectionModel *Model;
	acSelectionDataItem *Parent;
	QList<acSelectionDataItem*> Children;
	acSelectionModel::ItemType Type;
	pqServerManagerModelItem* Object;

	bool Visible;
	bool Selectable;

public:
	acSelectionDataItem(acSelectionModel* model,
		pqServerManagerModelItem* object,
		acSelectionModel::ItemType itemType)
		: QObject(model),
		Model(model),
		Parent(NULL),
		Type(itemType),
		Object(object),
		Selectable(true)
	{
	}
};

acSelectionModel::acSelectionModel(QObject *parent)
	: Superclass(parent),
	Root(new acSelectionDataItem(this, NULL, acSelectionModel::Invalid))
{

}

acSelectionModel::~acSelectionModel()
{
	delete this->Root;
}

acSelectionDataItem* acSelectionModel::createDataItem(
	pqServerManagerModelItem* item, acSelectionDataItem* subtreeRoot = NULL,
	acSelectionModel::ItemType type) const
{
	if (subtreeRoot == 0)
	{
		subtreeRoot = this->Root;
	}

	if (!item)
	{
		return 0;
	}

	if (subtreeRoot->Object == item && (type == acSelectionModel::Invalid || type == subtreeRoot->Type))
	{
		return subtreeRoot;
	}

	foreach(acSelectionDataItem* child, subtreeRoot->Children)
	{
		acSelectionDataItem* retVal = this->createDataItem(item, child, type);
		if (retVal && (type == acSelectionModel::Invalid || type == retVal->Type))
		{
			return retVal;
		}
	}
	return 0;
}


//-----------------------------------------------------------------------------
int acSelectionModel::rowCount(const QModelIndex& parentIndex) const
{
	if (parentIndex.isValid() && parentIndex.model() == this)
	{
		acSelectionDataItem* item =
			reinterpret_cast<acSelectionDataItem*>(parentIndex.internalPointer());
		return item->Children.size();
	}
	return this->Root->Children.size();
}

int acSelectionModel::columnCount(const QModelIndex&) const
{
	return 2;
}

bool acSelectionModel::hasChildren(const QModelIndex& parentIndex) const
{
	return this->rowCount(parentIndex) > 0;
}

QModelIndex acSelectionModel::index(int row, int column, const QModelIndex& parentIndex) const
{
	// Make sure the row and column number is within range.
	int rows = this->rowCount(parentIndex);
	int columns = this->columnCount(parentIndex);
	if (row < 0 || row >= rows || column < 0 || column >= columns)
	{
		return QModelIndex();
	}

	acSelectionDataItem* parentItem = 0;
	if (parentIndex.isValid())
	{
		parentItem = reinterpret_cast<acSelectionDataItem*>(parentIndex.internalPointer());
	}
	else
	{
		// The parent refers to the model root.
		parentItem = this->Root;
	}

	return this->createIndex(row, column, parentItem->Children[row]);
}

QModelIndex acSelectionModel::parent(const QModelIndex& idx) const
{
	if (idx.isValid() && idx.model() == this)
	{
		acSelectionDataItem* item =
			reinterpret_cast<acSelectionDataItem*>(idx.internalPointer());

		acSelectionDataItem* _parent = item->Parent;
		return this->getIndex(_parent);
	}

	return QModelIndex();
}

QVariant acSelectionModel::data(const QModelIndex& idx, int role) const
{
	//TODO: To be implemented
	return QVariant();
}

Qt::ItemFlags acSelectionModel::flags(const QModelIndex& index) const
{
	//TODO: To be implemented
	return 0;
}

QModelIndex acSelectionModel::getIndexFor(pqServerManagerModelItem* item) const
{
	//TODO: To be implemented
	return QModelIndex();
}

pqServerManagerModelItem* acSelectionModel::getItemFor(const QModelIndex&) const
{
	//TODO: To be implemented
	return NULL;
}

QModelIndex acSelectionModel::getIndex(acSelectionDataItem* dataItem) const
{
	//TODO: To be implemented
	return QModelIndex();
}
