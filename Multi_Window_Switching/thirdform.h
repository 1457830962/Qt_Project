#ifndef THIRDFORM_H
#define THIRDFORM_H
#include <QTimer>

#include <QWidget>

namespace Ui {
class ThirdForm;
}

class ThirdForm : public QWidget
{
    Q_OBJECT

public:
    explicit ThirdForm(QWidget *parent = 0);
    ~ThirdForm();
private slots:
    void doProcessTimeOut();
    void doProcessBacktoTwoForm(bool);

private:
    Ui::ThirdForm *ui;
    int count;
    QTimer *mytimer;
    void Init();//初始化函数

signals:
    void SignalShowTwoForm(QString);
};

#endif // THIRDFORM_H
