#include "twoform.h"
#include "ui_twoform.h"
#include <QMovie>
#include <QDebug>

TwoForm::TwoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TwoForm)
{
    ui->setupUi(this);
    Init();
}

TwoForm::~TwoForm()
{
    delete ui;
}


//////=========function=============//////
void TwoForm::Init()
{
    clickCount=0;
    g_ptf3=new ThirdForm();

    //动画显示
    QMovie *movie= new QMovie("/home/philip/Documents/Qt_Project/Qt_Project20180819/Multi_Window_Switching/timg.gif");
    movie->start();
    ui->label->setMovie(movie);
    ui->label->setScaledContents(true);
    //处理点击返回到第一个界面
    connect(ui->btn_back_one,SIGNAL(clicked(bool)),this,SLOT(doProcessClickBackOneForm(bool)));
    //点击跳转到第三个界面
    connect(ui->btn_to_third,SIGNAL(clicked(bool)),this,SLOT(doProcessClicktoThirdTorm(bool)));
    //接收第三个界面传递来信号
    connect(g_ptf3,SIGNAL(SignalShowTwoForm(QString)),this,SLOT(doProcessClinckThirdFormRequest(QString)));

}


//////=========slots=============//////
void TwoForm::doProcessClickBackOneForm(bool)
{
    clickCount++;//对点击返回第一个界面按钮次数进行统计
    //发射信号
    emit SignalShowOneForm(QString::number(clickCount));//发射信号
    this->hide();//点击返回第一个按钮后隐藏第二个界面
}

/////==================slots==================/////

void TwoForm::doProcessClicktoThirdTorm(bool)
{
    g_ptf3->show();//显示第三个界面
    this->hide();

}


////=======slots====================///

void TwoForm::doProcessClinckThirdFormRequest(QString str)
{
    qDebug()<<str<<endl;
    this->show();//显示第二个界面
}
