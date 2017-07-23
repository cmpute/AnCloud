#ifndef acLoadDataReaction_h
#define acLoadDataReaction_h

#include "acReaction.h"

/**
 * Reaction for opening data files
 */
class acLoadDataReaction : public acReaction
{
    Q_OBJECT
    typedef acReaction Superclass;

public:
    acLoadDataReaction(QAction *parent);

signals:
    void loadedData();

protected:
    virtual void onTriggered()
    {
        qDebug() << "[Open] Triggered!";
        // TODO: Not Implemented
    }

private:
    Q_DISABLE_COPY(acLoadDataReaction)
};

#endif // acLoadDataReaction_h
