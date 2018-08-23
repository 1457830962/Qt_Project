#include "thirdform.h"
#include "ui_thirdform.h"

ThirdForm::ThirdForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThirdForm)
{
    ui->setupUi(this);

    Init();
}

ThirdForm::~ThirdForm()
{
    delete ui;
}


////////==============function===========//
void ThirdForm::Init()
{
    count=0;
    mytimer = new QTimer(this);
    connect(mytimer,SIGNAL(timeout()),this,SLOT(doProcessTimeOut()));
    mytimer->start(1000);
    //点击返回第二个界面
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(doProcessBacktoTwoForm(bool)));

}



//记录界面时间
//=================slots===============================//
void ThirdForm::doProcessTimeOut()
{
    count++;//记录一下点击的次数
    if(count==1000)
    {
        count=0;
    }
    ui->lcdNumber->display(count);
}


//====================slots===============================//
void ThirdForm::doProcessBacktoTwoForm(bool)
{
    count++;
    emit SignalShowTwoForm(QString::number(count));//发射信号  通知显示第二个界面
    this->hide();//点击返回第二个界面按钮后隐藏第三个界面
}
