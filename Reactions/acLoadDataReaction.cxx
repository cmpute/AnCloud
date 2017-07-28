#include "acLoadDataReaction.h"

#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqCoreUtilities.h"
#include "pqObjectBuilder.h"
#include "pqPipelineRepresentation.h"
#include "pqPipelineSource.h"
#include "pqServer.h"
#include "pqSettings.h"
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

}

pqPipelineSource* acLoadDataReaction::loadData()
{
	/*pqServer *server = pqActiveObjects::instance().activeServer();
	vtkSMReaderFactory *readerFactory = vtkSMProxyManager::GetProxyManager()->GetReaderFactory();
	QString filters = readerFactory->GetSupportedFileTypes(server->session());*/

	const QString defaultPathToken = "AnCloud/OpenData/DefaultDir";

	QString filters = tr("PCD Data Files (*.pcd);;All Files (*)");
	pqSettings* settings = pqApplicationCore::instance()->settings();
	QString defaultDir = settings->value(defaultPathToken, QDir::homePath()).toString();
	QString fileName = QFileDialog::getOpenFileName(
		pqCoreUtilities::mainWidget(), tr("Open Data File"), defaultDir, filters);

	if (fileName.isEmpty())
		return NULL;
	settings->setValue(defaultPathToken, QFileInfo(fileName).absoluteDir().absolutePath());

	qDebug() << "Open File " << fileName;
	// TODO: Finish Data Reader
}