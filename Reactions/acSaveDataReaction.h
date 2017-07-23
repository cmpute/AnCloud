#ifndef acSaveDataReaction_h
#define acSaveDataReaction_h

#include "acReaction.h"

/**
 * Reaction for saving data files
 */
class acSaveDataReaction : public acReaction
{
    Q_OBJECT
    typedef acReaction Superclass;

public:
    acSaveDataReaction(QAction *parent);

protected:
    virtual void onTriggered()
    {
        qDebug() << "[Save] Triggered!";
        // TODO: Not Implemented
    }

private:
    Q_DISABLE_COPY(acSaveDataReaction)
};

#endif // acSaveDataReaction_h
