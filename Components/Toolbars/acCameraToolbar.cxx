#include "acCameraToolbar.h"
#include "ui_acCameraToolbar.h"

#include "pqActiveObjects.h"
#include "pqRenderView.h"
#include "pqCameraReaction.h"

void acCameraToolbar::constructor()
{
    Ui_acCameraToolbar ui;
    ui.setupUi(this);
    new pqCameraReaction(ui.actionResetCamera, pqCameraReaction::RESET_CAMERA);
    new pqCameraReaction(ui.actionZoomToData, pqCameraReaction::ZOOM_TO_DATA);
    new pqCameraReaction(ui.actionPositiveX, pqCameraReaction::RESET_POSITIVE_X);
    new pqCameraReaction(ui.actionNegativeX, pqCameraReaction::RESET_NEGATIVE_X);
    new pqCameraReaction(ui.actionPositiveY, pqCameraReaction::RESET_POSITIVE_Y);
    new pqCameraReaction(ui.actionNegativeY, pqCameraReaction::RESET_NEGATIVE_Y);
    new pqCameraReaction(ui.actionPositiveZ, pqCameraReaction::RESET_POSITIVE_Z);
    new pqCameraReaction(ui.actionNegativeZ, pqCameraReaction::RESET_NEGATIVE_Z);
	this->showOrientationAxesAction = ui.actionShowOrientationAxes;
    QObject::connect(this->showOrientationAxesAction, SIGNAL(toggled(bool)),
                     this, SLOT(showOrientationAxes(bool)));

    QObject::connect(&pqActiveObjects::instance(), SIGNAL(viewChanged(pqView*)),
                     this, SLOT(updateEnabledState()), Qt::QueuedConnection);
}

void acCameraToolbar::showOrientationAxes(bool show_axes)
{
    pqRenderView* renderView = qobject_cast<pqRenderView*>(pqActiveObjects::instance().activeView());
    if (renderView)
    {
        renderView->setOrientationAxesVisibility(show_axes);
        renderView->render();
    }
}

void acCameraToolbar::updateEnabledState()
{
    pqRenderView* renderView = qobject_cast<pqRenderView*>(pqActiveObjects::instance().activeView());
    this->showOrientationAxesAction->setChecked(
        renderView ? renderView->getOrientationAxesVisibility() : false);
}
