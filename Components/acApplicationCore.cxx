#include "acApplicationCore.h"
#include "acComponentsInit.h"

#include <QApplication>

acApplicationCore::acApplicationCore(QObject* parent)
    : Superclass(parent)
{
    // Initialize AnCloud resources
    acComponentsInit();

}

acApplicationCore::~acApplicationCore()
{
}

QPointer<acApplicationCore> acApplicationCoreInstance = NULL;
acApplicationCore* acApplicationCore::instance()
{
    if (!acApplicationCoreInstance)
        acApplicationCoreInstance = new acApplicationCore(pqApplicationCore::instance());

    return acApplicationCoreInstance;
}

QMainWindow* acApplicationCore::mainWindow = NULL;
QMainWindow* acApplicationCore::getMainWindow()
{
    if(acApplicationCore::mainWindow != NULL)
        return acApplicationCore::mainWindow; // we suppose that main window remain always the same
    foreach(QWidget *topWidget, QApplication::topLevelWidgets())
    {
		QMainWindow* window = qobject_cast<QMainWindow*>(topWidget);
        if (window)
        {
			acApplicationCore::mainWindow = window;
            return window;
        }
    }
    return NULL;
}
