#include "widget.h"
#include "ui_widget.h"
#include <QTime>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    Init();
}

Widget::~Widget()
{
    delete ui;
}

//初始化函数
void Widget::Init()
{
    num=0;
    currentNum.clear();//清空初始化
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//产生种子

    num=qrand()%100;
//    qDebug()<<num<<endl;
    //测试有无种子产生的区别
    // for(int i=0;i<10;i++)
    // {
    //   qDebug()<<qrand()%100;
    // }
    //进行帮定UI下面的按钮
    connect(ui->btn0,SIGNAL(clicked(bool)),this,SLOT(doProcessorClinckNum()));
    connect(ui->btn1,SIGNAL(clicked(bool)),this,SLOT(doProcessorClinckNum()));
    connect(ui->btn2,SIGNAL(clicked(bool)),this,SLOT(doProcessorClinckNum()));
    connect(ui->btn3,SIGNAL(clicked(bool)),this,SLOT(doProcessorClinckNum()));
    connect(ui->btn4,SIGNAL(clicked(bool)),this,SLOT(doProcessorClinckNum()));
    connect(ui->btn5,SIGNAL(clicked(bool)),this,SLOT(doProcessorClinckNum()));
    connect(ui->btn6,SIGNAL(clicked(bool)),this,SLOT(doProcessorClinckNum()));
    connect(ui->btn7,SIGNAL(clicked(bool)),this,SLOT(doProcessorClinckNum()));
    connect(ui->btn8,SIGNAL(clicked(bool)),this,SLOT(doProcessorClinckNum()));
    connect(ui->btn9,SIGNAL(clicked(bool)),this,SLOT(doProcessorClinckNum()));
}


//处理点击事件的槽函数
void Widget::doProcessorClinckNum()
{
    QString msg;
    QPushButton *btn=(QPushButton*)this->sender();
    if(btn==NULL)
        return;
    QString text=btn->text();
    if(currentNum.length()<1)
    {
        currentNum.append(text);
    }
    else if(currentNum.length()==1)
    {
        currentNum.append(text);
        //比较
        int v1=currentNum.toInt();
        if(v1<num)
        {
            msg.append("输入的值过小");
            currentNum.clear();
        }
        else if(v1==num)
        {
            msg.append("猜数正确!");
            num=qrand()%100;
            qDebug()<<num<<endl;
        }
        else {
            msg.append("输入的值过大");
        }
        ui->textEdit->append(msg);
        currentNum.clear();
    }
    else {
        currentNum. clear();
    }
    ui->lcdNumber->display(currentNum);
}

