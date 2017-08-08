#ifndef acSelectionManager_h
#define acSelectionManager_h

#include <pqOutputPort.h>

class acSelectionManager : public QObject
{
    Q_OBJECT

public:
    acSelectionManager();
    virtual ~acSelectionManager();

public slots:
    void onSelectionChanged(pqOutputPort*);

private slots:
	void onItemRemoved(pqServerManagerModelItem*);
	void onSourceAdded(pqPipelineSource*);
	void onSourceRemoved(pqPipelineSource*);

private:
    Q_DISABLE_COPY(acSelectionManager)
};

#endif // acSelectionManager_h