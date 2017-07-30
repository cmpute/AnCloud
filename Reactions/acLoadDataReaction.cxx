#include "acLoadDataReaction.h"

#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqCoreUtilities.h"
#include "pqObjectBuilder.h"
#include "pqPipelineRepresentation.h"
#include "pqPipelineSource.h"
#include "pqServer.h"
#include "pqSettings.h"
#include "pqStandardRecentlyUsedResourceLoaderImplementation.h"
#include "pqUndoStack.h"
#include "pqView.h"
#include "vtkDataObject.h"
#include "vtkPVArrayInformation.h"
#include "vtkPVDataInformation.h"
#include "vtkPVDataSetAttributesInformation.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMProxy.h"
#include "vtkSMProxyManager.h"
#include "vtkSMReaderFactory.h"
#include "vtkSMRepresentationProxy.h"
#include "vtkSMPVRepresentationProxy.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QDir>

acLoadDataReaction::acLoadDataReaction(QAction *parent)
    : Superclass(parent)
{
	QObject::connect(this, SIGNAL(dataLoaded(pqPipelineSource*)),
				     this, SLOT(onDataLoaded(pqPipelineSource*)));
}

pqPipelineSource* acLoadDataReaction::loadData()
{
	pqServer *server = pqActiveObjects::instance().activeServer();
	vtkSMReaderFactory *readerFactory = vtkSMProxyManager::GetProxyManager()->GetReaderFactory();
	QString filters = readerFactory->GetSupportedFileTypes(server->session());
	const QString defaultPathToken = "AnCloud/OpenData/DefaultDir";

	pqSettings* settings = pqApplicationCore::instance()->settings();
	QString defaultDir = settings->value(defaultPathToken, QDir::homePath()).toString();
	QStringList fileGroup = QFileDialog::getOpenFileNames(
		pqCoreUtilities::mainWidget(), tr("Open Data File"), defaultDir, filters);

	if (fileGroup.isEmpty())
		return NULL;
	QString fileName = fileGroup.last();

	qDebug() << "Open File " << fileGroup;
	settings->setValue(defaultPathToken, QFileInfo(fileName).absoluteDir().absolutePath());

	pqPipelineSource* reader;
	// Determine file reader
	if (readerFactory->CanReadFile(fileName.toUtf8().data(), server->session()))
	{
		// reader knows the type
		BEGIN_UNDO_SET("Create 'Reader'");
		reader = loadFile(fileGroup, QPair<QString, QString>(readerFactory->GetReaderName(), readerFactory->GetReaderGroup()));
		END_UNDO_SET();
	}

	return reader;
}

pqPipelineSource* acLoadDataReaction::loadFile(const QStringList& files, const QPair<QString, QString>& readerInfo)
{
	pqServer *server = pqActiveObjects::instance().activeServer();
	pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
	pqPipelineSource* reader = builder->createReader(readerInfo.second, readerInfo.first, files, server);

	if (reader)
	{
		pqStandardRecentlyUsedResourceLoaderImplementation::addDataFilesToRecentResources(
			server, files, reader->getProxy()->GetXMLGroup(), reader->getProxy()->GetXMLName());
		qDebug() << "Reader Created: " << reader->getProxy()->GetXMLName();
	}

	return reader;
}

void acLoadDataReaction::onDataLoaded(pqPipelineSource* source)
{
	pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();

	pqActiveObjects::instance().setActiveSource(source);
	pqPipelineRepresentation* repr = qobject_cast<pqPipelineRepresentation*>(
		builder->createDataRepresentation(
			source->getOutputPort(0), pqActiveObjects::instance().activeView()));
	if (!repr)
	{
		qWarning("Failed to create representation");
		return;
	}

	vtkSMPropertyHelper(repr->getProxy(), "Representation").Set("Points");
	vtkSMPropertyHelper(repr->getProxy(), "InterpolateScalarsBeforeMapping").Set(0);

	// color by "intensity" if array is present.
	vtkSMPVRepresentationProxy* pvrp = vtkSMPVRepresentationProxy::SafeDownCast(repr->getRepresentationProxy());
	vtkPVDataInformation* info = repr->getInputDataInformation();
	vtkPVArrayInformation* arrayInfo =
		info->GetPointDataInformation()->GetArrayInformation("intensity");
	if (arrayInfo != NULL && pvrp != NULL)
	{
		pvrp->SetScalarColoring("intensity", vtkDataObject::FIELD_ASSOCIATION_POINTS);
	}

	repr->getProxy()->UpdateVTKObjects();
	repr->renderViewEventually();
	pqActiveObjects::instance().activeView()->resetDisplay();
}
