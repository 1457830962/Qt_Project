#include "oneform.h"

#include "ui_oneform.h"
#include <QMovie>
#include <QDebug>

OneForm::OneForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OneForm)
{
    ui->setupUi(this);
    Init();
}

OneForm::~OneForm()
{
    delete ui;
}


//////================function============/////
void OneForm::Init()
{
    //动画显示
    QMovie *movie= new QMovie("/home/philip/Documents/Qt_Project/Qt_Project20180819/Multi_Window_Switching/timg.gif");
    movie->start();
    ui->label->setMovie(movie);
    ui->label->setScaledContents(true);

    //处理点击事件  绑定按钮处理界面转到第二个界面
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(doProcessClinckGotoTwoForm(bool)));
    //开始就对全局指针对象进行初始化
    g_ptf2 = new TwoForm();

    //接受第二个界面的显示请求
    connect(g_ptf2,SIGNAL(SignalShowOneForm(QString)),this,SLOT(doProcessClinckTwoFormRequest(QString)));

}

//////================slots============/////
void OneForm::doProcessClinckGotoTwoForm(bool)
{
    //创建第二个界面有两种方式：一、创建多个第二个界面；二、创建一个第二个见面
    //第一种利用动态创建指针  可创建多个
    //    TwoForm *ptf2=new TwoForm(/*this*/);  //非子父窗体  所以第二个界面不要利用this进行初始化  还可以利用全局对象进行界面的切换
    //    ptf2->show();                          //利用对象（注意这里的指针只能是用全局对象）进行show界面时不能show多个  而利用指针进行显示时可以show很多个


    //第二种创建窗口的方法 利用全局对象
    //    tf2.show();

    //第三种全局指针
        g_ptf2->show();
        this->hide();

}



//处理TwoForm的请求
 void OneForm::doProcessClinckTwoFormRequest(QString str)
 {
     qDebug()<<str<<endl;
     this->show();

 }
