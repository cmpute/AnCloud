#ifndef acDeleteReaction_h
#define acDeleteReaction_h

#include "acReaction.h"

class acDeleteReaction : public acReaction
{
    Q_OBJECT
    typedef acReaction Superclass;

public:
    acDeleteReaction(QAction* parent, bool delete_all = false);

    static void deleteAll();
    static void deleteSelected();
    static bool canDeleteSelected();

private:
    Q_DISABLE_COPY(acDeleteReaction)
};

#endif // acDeleteReaction_h