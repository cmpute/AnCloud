#include "acMainWindow.h"
#include "ui_acMainWindow.h"

#include "acMenuBuilders.h"

acMainWindow::acMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui_acMainWindow)
{
    this->ui->setupUi(this);

    // Setup menus
    acMainMenuBuilder::buildFileMenu(*this->ui->menuFile);
    acMainMenuBuilder::buildEditMenu(*this->ui->menuEdit);
    acMainMenuBuilder::buildViewMenu(*this->ui->menuView);
    acMainMenuBuilder::buildHelpMenu(*this->ui->menuHelp);
};

acMainWindow::~acMainWindow()
{
	delete this->ui;
}
