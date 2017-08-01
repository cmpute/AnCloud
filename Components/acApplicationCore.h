#ifndef acApplicationCore_h
#define acApplicationCore_h

#include <pqApplicationCore.h>

#include <QMainWindow>
#include <QPointer>

class acApplicationCore : public QObject
{
    Q_OBJECT
    typedef QObject Superclass;

public:
    ~acApplicationCore();

    static acApplicationCore* instance();

    static QMainWindow* getMainWindow();

private:
    acApplicationCore(QObject* parent = 0);

    Q_DISABLE_COPY(acApplicationCore)

    static QMainWindow* mainWindow;
};

#endif // acApplicationCore_h
