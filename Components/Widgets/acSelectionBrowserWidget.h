#ifndef acSelectionBrowserWidget_h
#define acSelectionBrowserWidget_h

#include "acSelectionModel.h"
#include <QTreeView>

class acSelectionBrowserWidget : public QTreeView
{
    Q_OBJECT
    typedef QTreeView Superclass;

public:
    acSelectionBrowserWidget(QWidget *parent = 0);
    virtual ~acSelectionBrowserWidget();

    /**
     * Used to monitor the key press events in the tree view.
     * Returns True if the event should not be sent to the object.
     */
    virtual bool eventFilter(QObject* object, QEvent* e);

signals:
    void deleteKeyPressed();

protected:
	acSelectionModel* Model;

private:
    Q_DISABLE_COPY(acSelectionBrowserWidget)
};

#endif // acSelectionBrowserWidget_h
