#ifndef acSelectionAdaptor_h
#define acSelectionAdaptor_h

#include <pqSelectionAdaptor.h>

// Reference to pqPipelineModelSelectionAdaptor
/* Adapt selectionModel of QTreeView to SelectionManager */
class acSelectionAdaptor : public pqSelectionAdaptor
{
	Q_OBJECT
	typedef pqSelectionAdaptor Superclass;

public:
	acSelectionAdaptor(QItemSelectionModel*);

protected:
	virtual QModelIndex mapFromItem(pqServerManagerModelItem*) const;
	virtual pqServerManagerModelItem* mapToItem(const QModelIndex& index) const;

private:
	Q_DISABLE_COPY(acSelectionAdaptor)
};

#endif // acSelectionAdaptor_h