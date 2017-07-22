#include "Components/acMainWindow.h"
#include <QApplication>

extern int qInitResources_icons();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("fusion");
    // Support high dpi displaying
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    qInitResources_icons();

    acMainWindow window;
    window.show();

    return app.exec();
}
