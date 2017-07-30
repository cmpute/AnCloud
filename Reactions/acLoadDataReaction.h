#ifndef acLoadDataReaction_h
#define acLoadDataReaction_h

#include "pqLoadDataReaction.h"
#include "acReaction.h"

// TODO: Support multiple files opening
/*
 * Reaction for opening data files
 */
class acLoadDataReaction : public acReaction
{
    Q_OBJECT
    typedef acReaction Superclass;

public:
    acLoadDataReaction(QAction *parent);

	static pqPipelineSource* loadData();

signals:
	/* Fired when a dataset is loaded by this reaction. */
	void dataLoaded(pqPipelineSource*);

protected:
    virtual void onTriggered()
    {
        qDebug() << "[Open] Triggered!";
		pqPipelineSource *source = acLoadDataReaction::loadData();
		if (source)
			emit this->dataLoaded(source);
    }

	static pqPipelineSource* loadFile(const QStringList& files, const QPair<QString, QString>& readerInfo);

private slots:
	void onDataLoaded(pqPipelineSource*);

private:
    Q_DISABLE_COPY(acLoadDataReaction)
};

#endif // acLoadDataReaction_h
