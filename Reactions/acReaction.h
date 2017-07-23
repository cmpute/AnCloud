#ifndef acReaction_h
#define acReaction_h

#include <QAction>
#include <QObject>
#include <QDebug>

class acReaction : public QObject
{
    Q_OBJECT
    typedef QObject Superclass;

public:
    /**
    * Constructor. Parent cannot be NULL.
    */
    acReaction(QAction *parent, Qt::ConnectionType type = Qt::AutoConnection);
    virtual ~acReaction();

    /**
    * Provides access to the parent action.
    */
    QAction* parentAction() const { return qobject_cast<QAction*>(this->parent()); }

protected slots:
    /**
     * Called when the action is triggered.
     */
    virtual void onTriggered() {}
    virtual void updateEnableState() {}
    // virtual void updateMasterEnableState(bool);

protected:
    bool IsMaster;

private:
    Q_DISABLE_COPY(acReaction)
};

#endif // acReaction_h
