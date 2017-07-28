#ifndef acLabelViewWidget_h
#define acLabelViewWidget_h

#include <QWidget>
#include <QTableView>

class acLabelViewWidget : public QWidget
{
    Q_OBJECT
    typedef QWidget Superclass;

public:
    acLabelViewWidget(QWidget *parent = NULL);
    ~acLabelViewWidget();

private:
    QTableView *View;
	Q_DISABLE_COPY(acLabelViewWidget);
};

#endif // acLabelViewWidget_h
