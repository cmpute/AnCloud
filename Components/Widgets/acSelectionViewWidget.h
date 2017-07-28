#ifndef acSelectionViewWidget_h
#define acSelectionViewWidget_h

#include "ui_acSelectionViewWidget.h"

#include <QWidget>
#include <QMenu>
#include <QPointer>
#include <QToolBar>
#include <QTreeView>

// Reference to pqViewFrame
class acSelectionViewWidget : public QWidget
{
	Q_OBJECT
    typedef QWidget Superclass;

public:
	acSelectionViewWidget(QWidget *parent = NULL);
	~acSelectionViewWidget();

private:
    Ui_acSelectionViewWidget *Ui;

	Q_DISABLE_COPY(acSelectionViewWidget);
};

#endif // acSelectionViewWidget_h