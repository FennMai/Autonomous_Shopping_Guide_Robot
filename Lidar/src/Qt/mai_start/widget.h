#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

//signals:
//    void pushbutton_textchanged();

private:
    QPushButton *pushButton1;
    Ui::Widget *ui;

private slots:
    void on_Button_closed_clicked();

    void pushbutton1_clicked();
    void on_Button_lidar_clicked();

};
#endif // WIDGET_H
