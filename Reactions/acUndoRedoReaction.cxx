#include "acUndoRedoReaction.h"

acUndoRedoReaction::acUndoRedoReaction(QAction *parent, bool isUndo)
    : Superclass(parent)
{
    this->IsUndo = isUndo;
    this->enable(false);
}
