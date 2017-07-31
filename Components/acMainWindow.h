#ifndef acMainWindow_h
#define acMainWindow_h

#include <QMainWindow>
#include <pqRenderView.h>

// Forward Qt class declarations
class Ui_acMainWindow;

class acMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit acMainWindow(QWidget *parent = 0);
  ~acMainWindow();

signals:
  void windowLoaded();

protected slots:

private:
  Ui_acMainWindow *Ui;
  pqRenderView *mainView;

  void paraviewInit();
};

#endif // acMainWindow_h
