#include "acSelectionAdaptor.h"
#include "Components/Widgets/acSelectionModel.h"

#include <QDebug>

acSelectionAdaptor::acSelectionAdaptor(QItemSelectionModel* selectionModel)
	: Superclass(selectionModel)
{
	Q_ASSERT(selectionModel != NULL);
	if (!qobject_cast<const acSelectionModel*>(this->getQModel()))
	{
		qWarning() << "QItemSelectionModel must be a selection model for"
			" acSelectionAdaptor.";
	}
}

QModelIndex acSelectionAdaptor::mapFromItem(pqServerManagerModelItem* item) const
{
	const acSelectionModel* pM = qobject_cast<const acSelectionModel*>(this->getQModel());
	return pM->getIndexFor(item);
}

pqServerManagerModelItem* acSelectionAdaptor::mapToItem(const QModelIndex& index) const
{
	const acSelectionModel* pM = qobject_cast<const acSelectionModel*>(this->getQModel());
	return pM->getItemFor(index);
}
