#ifndef TWOFORM_H
#define TWOFORM_H

#include <QWidget>
#include <thirdform.h>

namespace Ui {
class TwoForm;
}

class TwoForm : public QWidget
{
    Q_OBJECT

public:
    explicit TwoForm(QWidget *parent = 0);
    ~TwoForm();

private:
    Ui::TwoForm *ui;
    void Init();
    int clickCount;

    ThirdForm *g_ptf3;//指针对象

private slots:
    void doProcessClickBackOneForm(bool);
    void doProcessClicktoThirdTorm(bool);
   void doProcessClinckThirdFormRequest(QString);
signals:
    void SignalShowOneForm(QString);
};

#endif // TWOFORM_H
