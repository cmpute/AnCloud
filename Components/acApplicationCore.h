#ifndef acApplicationCore_h
#define acApplicationCore_h

#include "Core/acSelectionManager.h"

#include <pqApplicationCore.h>

#include <QMainWindow>
#include <QPointer>

/* Provide functions in addition to pqApplicationCore */
class acApplicationCore : public QObject
{
    Q_OBJECT
    typedef QObject Superclass;

public:
    ~acApplicationCore();

    static acApplicationCore* instance();

    static QMainWindow* getMainWindow();

private:
    acApplicationCore(QObject* parent = 0);

    Q_DISABLE_COPY(acApplicationCore)

    static QMainWindow *mainWindow;
	acSelectionManager *selectionManager;
};

#endif // acApplicationCore_h
