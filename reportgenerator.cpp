#include "reportgenerator.h"
#include "ui_reportgenerator.h"

ReportGenerator::ReportGenerator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReportGenerator)
{
    ui->setupUi(this);
}

ReportGenerator::~ReportGenerator()
{
    delete ui;
}
