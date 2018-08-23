#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMovie>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    ui->label->hide();//栅格布局后  隐藏任何一个控件后，另外一个控件都将变为全屏大小

 Init();
}

Widget::~Widget()
{
    delete ui;
}


//==========
void Widget::Init()
{
    count=3;
    ui->label->hide();//事先隐藏label控件
    //倒计时
    mytimer = new QTimer(this);
    connect(mytimer,SIGNAL(timeout()),this,SLOT(doProcessorTimeOut()));
    mytimer->start(1000);//每一秒钟启动一次计数

    //播放动画
    movie = new QMovie(this);
    movie->setFileName("./timg.gif");
    ui->label->setMovie(movie);
    ui->label->setScaledContents(true);//label中的内容跟随窗体变大而变大  注意了
}



void Widget::doProcessorTimeOut()
{
    count--;
    if(count==0)
    {
        mytimer->stop();
        movie->start();
        ui->label->show();
        ui->lcdNumber->hide();
        this->showFullScreen();//进行全屏显示
    }
    ui->lcdNumber->display(count);
       qDebug()<<count<<endl;//打印计时

}
