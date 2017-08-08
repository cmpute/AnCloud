#ifndef acSelectionModel_h
#define acSelectionModel_h

#include <pqServerManagerModelItem.h>
#include <QAbstractItemModel.h>

class acSelectionModel : public QAbstractItemModel
{
    Q_OBJECT;
    typedef QAbstractItemModel Superclass;

public:
	enum ItemType
	{
		Invalid = -1,
		Source = 0,
		Selection_Unlabelled,
		Selection_Labelled,
		CrossSelection_Unlabelled,
		CrossSelection_Labelled
	};
    acSelectionModel(QObject* parent = NULL);
    virtual ~acSelectionModel();

	////////////// QAbstractItemModel Methods //////////////
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) const;
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex& index) const;
	virtual Qt::ItemFlags flags(const QModelIndex& index) const;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

	////////////// Selection Mapping /////////////
	pqServerManagerModelItem* getItemFor(const QModelIndex&) const;
	QModelIndex getIndexFor(pqServerManagerModelItem* item) const;

private:
	friend class acSelectionDataItem;

	acSelectionDataItem* Root;
	acSelectionDataItem* createDataItem(pqServerManagerModelItem* item,
		acSelectionDataItem* subtreeRoot, ItemType type = Invalid) const;

	QModelIndex getIndex(acSelectionDataItem* item) const;

    Q_DISABLE_COPY(acSelectionModel)
};

#endif // acSelectionModel_h
