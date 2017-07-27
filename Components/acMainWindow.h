#ifndef acMainWindow_h
#define acMainWindow_h

#include <QMainWindow>

// Forward Qt class declarations
class Ui_acMainWindow;

class acMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit acMainWindow(QWidget *parent = 0);
  ~acMainWindow();

protected:

protected slots:

private:
  Ui_acMainWindow *ui;
};

#endif // acMainWindow_h
