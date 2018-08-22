#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    void Init();
    int num;
    QString currentNum;
private slots:
    void doProcessorClinckNum(); //处理点击事件的槽函数
};

#endif // WIDGET_H
