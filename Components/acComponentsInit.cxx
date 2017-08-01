#include "acComponentsInit.h"
#include <pqCoreInit.h>

#include <QObject> // for Q_INIT_RESOURCE

void acComponentsInit()
{
#ifndef ANCLOUD_BUILD_SHARED_LIBS
    // init dependents
    pqCoreInit();

    // init resources
    Q_INIT_RESOURCE(acIcons);
#endif
}
