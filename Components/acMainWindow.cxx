#include "acMainWindow.h"
#include "ui_acMainWindow.h"

#include <pqActiveObjects.h>
#include <pqApplicationCore.h>
#include <pqAutoLoadPluginXMLBehavior.h>
#include <pqCommandLineOptionsBehavior.h>
#include <pqCrashRecoveryBehavior.h>
#include <pqDataTimeStepBehavior.h>
#include <pqDefaultViewBehavior.h>
#include <pqInterfaceTracker.h>
#include <pqLockPanelsBehavior.h>
#include <pqViewMenuManager.h>
#include <pqObjectBuilder.h>
#include "pqObjectPickingBehavior.h"
#include <pqPersistentMainWindowStateBehavior.h>
#include <pqPythonShellReaction.h>
#include <pqQtMessageHandlerBehavior.h>
#include <pqRenderViewSelectionReaction.h>
#include <pqServer.h>
#include <pqSettings.h>
#include <pqSpreadSheetView.h>
#include <pqSpreadSheetVisibilityBehavior.h>
#include <pqSpreadSheetViewDecorator.h>
#include <pqStandardPropertyWidgetInterface.h>
#include <pqStandardViewFrameActionsImplementation.h>
#include <vtkPVPlugin.h>
#include <vtkSMProxyManager.h>
#include <vtkSMReaderFactory.h>
#include <vtkSMSessionProxyManager.h>
#include <vtkSMPropertyHelper.h>
#include <QtGlobal>

#include "acMenuBuilders.h"

PV_PLUGIN_IMPORT_INIT(PCDReader);

///////////// TEST ////////////////
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSMRenderViewProxy.h"
///////////////////////////////////

acMainWindow::acMainWindow(QWidget *parent) :
    QMainWindow(parent),
    Ui(new Ui_acMainWindow),
	mainView(NULL)
{
    this->Ui->setupUi(this);
	this->paraviewInit();
	PV_PLUGIN_IMPORT(PCDReader);

    this->setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);
	this->tabifyDockWidget(this->Ui->selectionViewDock, this->Ui->labelViewDock);
	// Set up the dock window corners to give the vertical docks more room.
  	this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

    // Setup menus
    acMainMenuBuilder::buildFileMenu(*this->Ui->menuFile);
    acMainMenuBuilder::buildEditMenu(*this->Ui->menuEdit);
    acMainMenuBuilder::buildHelpMenu(*this->Ui->menuHelp);
	new pqViewMenuManager(this, this->Ui->menuView);
};

void acMainWindow::paraviewInit()
{
	pqApplicationCore *core = pqApplicationCore::instance();
	// Define application behaviours
	// TODO: Add behaviours
	new pqLockPanelsBehavior(this); // Enable locking panels

	// Define supported files
	vtkSMReaderFactory *readerFactory = vtkSMProxyManager::GetProxyManager()->GetReaderFactory();
	readerFactory->AddReaderToWhitelist("sources", "PCDReader");

	// Connect to builtin server
	pqObjectBuilder *builder = core->getObjectBuilder();
	pqServer *server = builder->createServer(pqServerResource("builtin:"));
	pqActiveObjects::instance().setActiveServer(server);

	// Set default render view settings
	vtkSMSessionProxyManager* pxm = vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager();
	vtkSMProxy* renderviewsettings = pxm->GetProxy("RenderViewSettings");
	Q_ASSERT(renderviewsettings);

	vtkSMPropertyHelper(renderviewsettings, "ResolveCoincidentTopology").Set(0);

	// Create a default view.
	pqRenderView* view = qobject_cast<pqRenderView*>(builder->createView(pqRenderView::renderViewType(), server));
	Q_ASSERT(view);
	this->mainView = view;

	vtkSMPropertyHelper(view->getProxy(), "CenterAxesVisibility").Set(0);
	double bgcolor[3] = { 0, 0, 0 };
	vtkSMPropertyHelper(view->getProxy(), "Background").Set(bgcolor, 3);
	// MultiSamples doesn't work, we need to set that up before registering the proxy.
	//vtkSMPropertyHelper(view->getProxy(),"MultiSamples").Set(1);
	view->getProxy()->UpdateVTKObjects();

	// Add to the window
	this->setCentralWidget(view->widget());
	pqActiveObjects::instance().setActiveView(view);

	// TODO: Add SpreadSheet View
}

acMainWindow::~acMainWindow()
{
	delete this->Ui;
}
