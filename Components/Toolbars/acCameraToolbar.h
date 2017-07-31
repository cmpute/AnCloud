#ifndef acCameraToolbar_h
#define acCameraToolbar_h

#include <QToolBar>

class acCameraToolbar : public QToolBar
{
    Q_OBJECT
    typedef QToolBar Superclass;

public:
    acCameraToolbar(const QString& title, QWidget* parentObject = 0)
        : Superclass(title, parentObject)
    {
        this->constructor();
    }
    acCameraToolbar(QWidget* parentObject = 0)
        : Superclass(parentObject)
    {
        this->constructor();
    }

protected slots:
    void updateEnabledState();
    void showOrientationAxes(bool);

private:
    Q_DISABLE_COPY(acCameraToolbar)
    void constructor();

    QAction* showOrientationAxesAction;
};

#endif
