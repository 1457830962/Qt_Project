#include "widget.h"
#include "ui_widget.h"
#include <QIcon>
#include <QSize>
#include <QMenu>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //设置图标
    ui->toolButton->setIcon(QIcon("../ToolBar/3.png"));
    ui->toolButton->setIconSize(QSize(5,5));

    QMenu *menu =new QMenu(this);
    menu->addAction(QIcon("../ToolBar/3.png"),"声音",this,"");
    ui->toolButton->setMenu(menu);
}

Widget::~Widget()
{
    delete ui;
}
