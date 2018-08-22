#ifndef ONEFORM_H
#define ONEFORM_H

#include "twoform.h"
#include <QWidget>

namespace Ui {
class OneForm;
}

class OneForm : public QWidget
{
    Q_OBJECT

public:
    explicit OneForm(QWidget *parent = 0);
    ~OneForm();

private:
    Ui::OneForm *ui;

    void Init();
     TwoForm tf2;//对象
     TwoForm *g_ptf2;//指针对象

    //思路：需要的一个槽处理点击事件
private slots:
    void doProcessClinckGotoTwoForm(bool);

    void doProcessClinckTwoFormRequest(QString);


};

#endif // ONEFORM_H
