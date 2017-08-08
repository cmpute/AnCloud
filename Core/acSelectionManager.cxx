#include "acSelectionManager.h"

#include <pqApplicationCore.h>
#include <pqServerManagerModel.h>
#include <pqPipelineSource.h>
#include <QDebug>

acSelectionManager::acSelectionManager()
{
	pqApplicationCore *core = pqApplicationCore::instance();
	pqServerManagerModel *model = core->getServerManagerModel();

	QObject::connect(model, SIGNAL(sourceAdded(pqPipelineSource*)),
		this, SLOT(onSourceAdded(pqPipelineSource*)));
	QObject::connect(model, SIGNAL(sourceRemoved(pqPipelineSource*)),
		this, SLOT(onSourceRemoved(pqPipelineSource*)));
	QObject::connect(model, SIGNAL(itemRemoved(pqServerManagerModelItem*)),
		this, SLOT(onItemRemoved(pqServerManagerModelItem*)));

	pqApplicationCore::instance()->registerManager("acSelectionManager", this);
	qDebug() << "acSelectionManager registered!";
}
acSelectionManager::~acSelectionManager()
{
	pqApplicationCore::instance()->unRegisterManager("acSelectionManager");
}

void acSelectionManager::onSelectionChanged(pqOutputPort *port)
{
	qDebug() << "Selection Changed";
}

void acSelectionManager::onItemRemoved(pqServerManagerModelItem *item)
{
	qDebug() << "Item Removed";
}

void acSelectionManager::onSourceAdded(pqPipelineSource *source)
{
	qDebug() << "Source Added";
}

void acSelectionManager::onSourceRemoved(pqPipelineSource *source)
{
	qDebug() << "Source Removed";
}
