#ifndef acSelectionToolbar_h
#define acSelectionToolbar_h

#include <QPointer.h>
#include <QShortcut>
#include <QToolBar>

#include "ui_acSelectionToolbar.h"

// Reference to pqStandardViewFrameActionsImplementation
class acSelectionToolbar : public QToolBar
{
    Q_OBJECT
    typedef QToolBar Superclass;

public:
    acSelectionToolbar(const QString& title, QWidget* parent = 0)
        : Superclass(title, parent), Ui(new Ui_acSelectionToolbar)
    {
        this->constructor();
    }
    acSelectionToolbar(QWidget* parent = 0)
        : Superclass(parent), Ui(new Ui_acSelectionToolbar)
    {
        this->constructor();
    }

protected slots:
    /*
    * If a QAction is added to an exclusive QActionGroup, then a checked action
    * cannot be unchecked by clicking on it. We need that to work. Hence, we
    * manually manage the exclusivity of the action group.
    */
    void manageGroupExclusivity(QAction*);
    
    /*
    * A slot called when any action that can be "cancelled" with Esc is toggled.
    */
    void escapeableActionToggled(bool checked);

    /*
    * A slot called when an interactive selection is toggled
    */
    void interactiveSelectionToggled(bool checked);

    /*
    * slots for various shortcuts.
    */
    void escTriggered();

	/*
	 * Setup after pqRenderView is created. 
	 */
	void setupSelectButtons();

private:
    Q_DISABLE_COPY(acSelectionToolbar)

    void constructor();
	Ui_acSelectionToolbar *Ui;
	QActionGroup* modeGroup;

	QPointer<QShortcut> ShortCutSurfaceCells;
	QPointer<QShortcut> ShortCutSurfacePoints;
	QPointer<QShortcut> ShortCutFrustumCells;
	QPointer<QShortcut> ShortCutFrustumPoints;
	QPointer<QShortcut> ShortCutBlocks;
	QPointer<QShortcut> ShortCutEsc;
};

#endif // acSelectionToolbar_h