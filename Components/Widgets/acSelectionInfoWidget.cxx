#include "acSelectionInfoWidget.h"

acSelectionInfoWidget::acSelectionInfoWidget(QWidget *parent)
    : Superclass(parent),
    Ui(new Ui_acSelectionInfoWidget)
{
    this->Ui->setupUi(this);
}

acSelectionInfoWidget::~acSelectionInfoWidget()
{
    delete this->Ui;
}

void acSelectionInfoWidget::onCurrentChanged(const QModelIndex &current)
{
    // TODO: Finish changed event
}