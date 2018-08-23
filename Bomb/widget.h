#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>

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
    int count;//用于计时
    QTimer *mytimer;
    QMovie *movie;//定义为全局

private slots:
    void doProcessorTimeOut();
};

#endif // WIDGET_H
