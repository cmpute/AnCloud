#ifndef acUndoRedoReaction_h
#define acUndoRedoReaction_h

#include "acReaction.h"

/**
 * Reaction for undo/redo
 */
class acUndoRedoReaction : public acReaction
{
    Q_OBJECT
    typedef acReaction Superclass;

public:
    acUndoRedoReaction(QAction *parent, bool isUndo);

protected slots:
    void enable(bool) { /* TODO: Not Implemented */ };

protected:
    virtual void onTriggered()
    {
        if(this->IsUndo)
        {
            qDebug() << "[Undo] Triggered!";
            // TODO: Not Implemented
        }
        else
        {
            qDebug() << "[Redo] Triggered!";
            // TODO: Not Implemented
        }
    }

private:
    Q_DISABLE_COPY(acUndoRedoReaction)

    bool IsUndo;
};

#endif // acUndoRedoReaction_h
