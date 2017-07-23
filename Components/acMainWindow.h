#ifndef acMainWindow_h
#define acMainWindow_h

#include "vtkSmartPointer.h"    // Required for smart pointer internal ivars.
#include <QMainWindow>

// Forward Qt class declarations
class Ui_acMainWindow;

// Forward VTK class declarations
class vtkQtTableView;


class acMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit acMainWindow(QWidget *parent = 0);
  ~acMainWindow();

protected:

protected slots:

private:
  vtkSmartPointer<vtkQtTableView> TableView;

  Ui_acMainWindow *ui;
};

#endif // acMainWindow_h
