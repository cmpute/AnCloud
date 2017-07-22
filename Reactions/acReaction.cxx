#include "acReaction.h"

acReaction::acReaction(QAction* parent, Qt::ConnectionType type)
    : Superclass(parent)
{
    Q_ASSERT(parent != NULL);

    QObject::connect(parent, SIGNAL(triggered(bool)), this, SLOT(onTriggered()), type);
}

acReaction::~acReaction() {}
