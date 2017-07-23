#ifndef acMainToolbar_h
#define acMainToolbar_h

#include <QToolBar>

class acMainToolbar : public QToolBar
{
    Q_OBJECT
    typedef QToolBar Superclass;

public:
    acMainToolbar(const QString& title, QWidget* parent = 0)
        : Superclass(title, parent)
    {
        this->constructor();
    }
    acMainToolbar(QWidget* parent = 0)
        : Superclass(parent)
    {
        this->constructor();
    }
private:
    Q_DISABLE_COPY(acMainToolbar)

    void constructor();
};

#endif // acMainToolbar_h