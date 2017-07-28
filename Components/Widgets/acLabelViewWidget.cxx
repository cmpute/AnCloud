#include "acLabelViewWidget.h"

#include <QVBoxLayout>

acLabelViewWidget::acLabelViewWidget(QWidget *parent)
    : Superclass(parent),
    View(new QTableView(this))
{
    QVBoxLayout* _layout = new QVBoxLayout(this);
    if (_layout)
    {
        _layout->setMargin(0);
        _layout->addWidget(this->View);
    }
}

acLabelViewWidget::~acLabelViewWidget()
{
    delete this->View;
}