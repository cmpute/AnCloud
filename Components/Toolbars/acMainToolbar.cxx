#include "acMainToolbar.h"
#include "ui_acMainToolbar.h"

#include "../../Reactions/acLoadDataReaction.h"
#include "../../Reactions/acSaveDataReaction.h"
#include "../../Reactions/acUndoRedoReaction.h"

#include <QToolButton>

void acMainToolbar::constructor()
{
    Ui_acMainToolbar ui;
    ui.setupUi(this);
    new acLoadDataReaction(ui.actionOpenData);
    new acSaveDataReaction(ui.actionSaveData);
    new acUndoRedoReaction(ui.actionUndo, true);
    new acUndoRedoReaction(ui.actionRedo, false);
}