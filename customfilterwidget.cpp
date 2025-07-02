#include "customfilterwidget.h"
#include "ui_customfilterwidget.h"

CustomFilterWidget::CustomFilterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomFilterWidget)
{
    ui->setupUi(this);
}

CustomFilterWidget::~CustomFilterWidget()
{
    delete ui;
}
