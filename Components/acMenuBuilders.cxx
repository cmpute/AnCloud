#include "acMenuBuilders.h"

#include "ui_acFileMenu.h"
#include "ui_acEditMenu.h"
#include "ui_acHelpMenu.h"

#include "../Reactions/acLoadDataReaction.h"
#include "../Reactions/acSaveDataReaction.h"
#include "../Reactions/acUndoRedoReaction.h"

void acMainMenuBuilder::buildFileMenu(QMenu &menu)
{
    Ui_acFileMenu ui;
    ui.setupUi(&menu);

    QObject::connect(ui.actionExit, SIGNAL(triggered()),
                     QApplication::instance(), SLOT(closeAllWindows()));
    new acLoadDataReaction(ui.actionOpenData);
    new acSaveDataReaction(ui.actionSaveData);
}

void acMainMenuBuilder::buildEditMenu(QMenu &menu)
{
    Ui_acEditMenu ui;
    ui.setupUi(&menu);

    new acUndoRedoReaction(ui.actionUndo, true);
    new acUndoRedoReaction(ui.actionRedo, false);
}

void acMainMenuBuilder::buildHelpMenu(QMenu &menu)
{
    Ui_acHelpMenu ui;
    ui.setupUi(&menu);
}
