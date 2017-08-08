#include "acSelectionBrowserWidget.h"
#include "../acSelectionAdaptor.h"

#include <QKeyEvent>

acSelectionBrowserWidget::acSelectionBrowserWidget(QWidget *parent)
    : Superclass(parent), Model(new acSelectionModel(this))
{
    this->installEventFilter(this);
	
	this->setModel(this->Model);
	this->setSelectionModel(new QItemSelectionModel(this->Model, this));
	new acSelectionAdaptor(this->selectionModel());
}

acSelectionBrowserWidget::~acSelectionBrowserWidget()
{
    
}

bool acSelectionBrowserWidget::eventFilter(QObject* object, QEvent* eventArg)
{
    if (object == this && eventArg->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(eventArg);
        if (keyEvent->key() == Qt::Key_Delete)
        {
            emit this->deleteKeyPressed();
        }
    }

    return this->Superclass::eventFilter(object, eventArg);
}
