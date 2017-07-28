#include "acSelectionViewWidget.h"

acSelectionViewWidget::acSelectionViewWidget(QWidget *parent)
    : Superclass(parent),
    Ui(new Ui_acSelectionViewWidget)
{
    this->Ui->setupUi(this);
}

acSelectionViewWidget::~acSelectionViewWidget()
{
    delete this->Ui;
}
