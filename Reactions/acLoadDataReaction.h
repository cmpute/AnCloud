#ifndef acLoadDataReaction_h
#define acLoadDataReaction_h

#include "pqLoadDataReaction.h"
#include "acReaction.h"

/**
 * Reaction for opening data files
 */
class acLoadDataReaction : public pqLoadDataReaction
{
    Q_OBJECT
    typedef pqLoadDataReaction Superclass;

public:
    acLoadDataReaction(QAction *parent);

	static pqPipelineSource* loadData();

//signals void loadedData(source) inherited.

protected:
    virtual void onTriggered()
    {
        qDebug() << "[Open] Triggered!";
		pqPipelineSource *source = acLoadDataReaction::loadData();
		if (source)
			emit this->loadedData(source);
    }

private:
    Q_DISABLE_COPY(acLoadDataReaction)
};

#endif // acLoadDataReaction_h
