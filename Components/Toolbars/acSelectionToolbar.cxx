#include "acSelectionToolbar.h"

#include <pqActiveObjects.h>
#include <pqRenderView.h>
#include <pqRenderViewSelectionReaction.h>
#include <pqView.h>
#include <vtkSMInteractiveSelectionPipeline.h>
#include <vtkSMRenderViewProxy.h>
#include <vtkSMTooltipSelectionPipeline.h>

#include <QToolButton>
#include <QMainWindow>

void acSelectionToolbar::constructor()
{
	this->Ui->setupUi(this);

	QMainWindow* mainWindow;
    foreach(QWidget *topWidget, QApplication::topLevelWidgets())
    {
		mainWindow = qobject_cast<QMainWindow*>(topWidget);
        if (mainWindow)
        {
			break;
        }
    }
    this->ShortCutEsc = new QShortcut(QKeySequence(Qt::Key_Escape), mainWindow);
    this->ShortCutEsc->setEnabled(false);
    this->connect(this->ShortCutEsc, SIGNAL(activated()), SLOT(escTriggered()));

    // Setup modifiers
    this->modeGroup = new QActionGroup(this);
    this->Ui->actionAddSelection->setData(QVariant(pqView::PV_SELECTION_ADDITION));
    this->Ui->actionSubtractSelection->setData(QVariant(pqView::PV_SELECTION_SUBTRACTION));
    this->Ui->actionToggleSelection->setData(QVariant(pqView::PV_SELECTION_TOGGLE));
    modeGroup->addAction(this->Ui->actionAddSelection);
    modeGroup->addAction(this->Ui->actionSubtractSelection);
    modeGroup->addAction(this->Ui->actionToggleSelection);
    this->connect(modeGroup, SIGNAL(triggered(QAction*)), SLOT(manageGroupExclusivity(QAction*)));

	this->connect(mainWindow, SIGNAL(windowLoaded()), SLOT(setupSelectButtons()));
}

void acSelectionToolbar::setupSelectButtons()
{
	// Setup selection buttons
	pqRenderView *view = qobject_cast<pqRenderView*>(pqActiveObjects::instance().activeView());

	new pqRenderViewSelectionReaction(this->Ui->actionSelectFrustumPoints, view,
		pqRenderViewSelectionReaction::SELECT_FRUSTUM_POINTS, modeGroup);
	this->connect(this->Ui->actionSelectFrustumPoints, SIGNAL(toggled(bool)),
		SLOT(escapeableActionToggled(bool)));

	new pqRenderViewSelectionReaction(this->Ui->actionSelectLassoPoints, view,
		pqRenderViewSelectionReaction::SELECT_SURFACE_POINTS_POLYGON, modeGroup);
	this->connect(this->Ui->actionSelectLassoPoints, SIGNAL(toggled(bool)),
		SLOT(escapeableActionToggled(bool)));

	new pqRenderViewSelectionReaction(this->Ui->actionSelectPointsInteractive, view,
		pqRenderViewSelectionReaction::SELECT_SURFACE_POINTS_INTERACTIVELY, modeGroup);
	this->connect(this->Ui->actionSelectPointsInteractive, SIGNAL(toggled(bool)),
		SLOT(escapeableActionToggled(bool)));
	this->connect(this->Ui->actionSelectPointsInteractive, SIGNAL(toggled(bool)),
		SLOT(interactiveSelectionToggled(bool)));

	new pqRenderViewSelectionReaction(this->Ui->actionHoveringPoints, view,
		pqRenderViewSelectionReaction::SELECT_SURFACE_POINTS_TOOLTIP);
	this->connect(this->Ui->actionHoveringPoints, SIGNAL(toggled(bool)),
		SLOT(escapeableActionToggled(bool)));
	this->connect(this->Ui->actionHoveringPoints, SIGNAL(toggled(bool)),
		SLOT(interactiveSelectionToggled(bool)));
}

void acSelectionToolbar::manageGroupExclusivity(QAction* curAction)
{
    if (!curAction || !curAction->isChecked())
    {
        return;
    }

    QActionGroup* group = qobject_cast<QActionGroup*>(this->sender());
    foreach (QAction* groupAction, group->actions())
    {
        if (groupAction != curAction && groupAction->isChecked())
        {
			groupAction->setChecked(false);
        }
    }
}

void acSelectionToolbar::escapeableActionToggled(bool checked)
{
    // If a "selection mode" begins, we will enable the ShortCutEsc to start monitoring
    // the Esc key to end selection.
    // If "selection mode" ends (due to one reason or another) and the selection that
    // ended was indeed the one which we're monitoring the Esc key for, then we
    // disable the Esc shortcut since it is no longer needed. This disabling
    // ensure that the shortcut doesn't eat away Esc keys which interferes with
    // the Esc key in the Search box, for example.
    QAction* actn = qobject_cast<QAction*>(this->sender());
    if (!actn || !actn->isEnabled() || !actn->isCheckable())
    {
        return;
    }

    if (!checked)
    {
        if (this->ShortCutEsc->property("PV_ACTION").value<QObject*>() == actn)
        {
			this->ShortCutEsc->setEnabled(false);
        }
        return;
    }

    // User has entered into a selection mode. Let's add a shortcut to "catch" the
    // Esc key.
    Q_ASSERT(checked && actn->isCheckable());
    this->ShortCutEsc->setEnabled(true);
    this->ShortCutEsc->setProperty("PV_ACTION", QVariant::fromValue<QObject*>(actn));
}

void acSelectionToolbar::escTriggered()
{
    QAction* actn = qobject_cast<QAction*>(
        this->ShortCutEsc->property("PV_ACTION").value<QObject*>());
    if (actn && actn->isChecked() && actn->isEnabled())
    {
        actn->trigger();
    }
    // this is not necessary for the most part, but just to be on the safe side.
    this->ShortCutEsc->setEnabled(false);
}

void acSelectionToolbar::interactiveSelectionToggled(bool checked)
{
    if (!checked)
    {
        vtkSMInteractiveSelectionPipeline::GetInstance()->Hide(
            vtkSMRenderViewProxy::SafeDownCast(pqActiveObjects::instance().activeView()->getViewProxy()));
        vtkSMTooltipSelectionPipeline::GetInstance()->Hide(
            vtkSMRenderViewProxy::SafeDownCast(pqActiveObjects::instance().activeView()->getViewProxy()));
    }
}
