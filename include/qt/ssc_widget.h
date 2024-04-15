#ifndef SSC_WIDGET_H
#define SSC_WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class SSC_Widget; }
QT_END_NAMESPACE

class SSC_Widget : public QWidget
{
    Q_OBJECT

public:
    SSC_Widget(QWidget *parent = nullptr);
    ~SSC_Widget();

private:
    Ui::SSC_Widget *ui;
};
#endif // SSC_WIDGET_H
