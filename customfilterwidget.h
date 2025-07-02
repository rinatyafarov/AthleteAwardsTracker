#ifndef CUSTOMFILTERWIDGET_H
#define CUSTOMFILTERWIDGET_H

#include <QWidget>

namespace Ui {
class CustomFilterWidget;
}

class CustomFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomFilterWidget(QWidget *parent = nullptr);
    ~CustomFilterWidget();

private:
    Ui::CustomFilterWidget *ui;
};

#endif // CUSTOMFILTERWIDGET_H
