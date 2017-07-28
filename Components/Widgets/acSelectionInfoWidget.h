#ifndef acSelectionInfoWidget_h
#define acSelectionInfoWidget_h

#include "ui_acSelectionInfoWidget.h"

#include <QWidget>
#include <QModelIndex>
#include <QPointer>

// Reference to pqProxyInformationWidget and pqFindDataSelectionDisplayFrame
class acSelectionInfoWidget : public QWidget
{
    Q_OBJECT
    typedef QWidget Superclass;

public:
    acSelectionInfoWidget(QWidget *parent = NULL);
    ~acSelectionInfoWidget();

protected slots:
    void onCurrentChanged(const QModelIndex &current);

private:
    Ui_acSelectionInfoWidget *Ui;

	Q_DISABLE_COPY(acSelectionInfoWidget);
};

#endif // acSelectionInfoWidget_h