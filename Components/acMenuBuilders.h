#ifndef acMenuBuilders_h
#define acMenuBuilders_h

#include <QMenu>

/**
 * acMainMenuBuilder provides helper methods to associate menu actions in acMainWindow
 * It use builder ui files to generate UI.
 */
class acMainMenuBuilder
{
public:
    static void buildFileMenu(QMenu &menu);
    static void buildEditMenu(QMenu &menu);
    static void buildViewMenu(QMenu &menu);
    static void buildHelpMenu(QMenu &menu);
};

#endif // acMenuBuilders_h