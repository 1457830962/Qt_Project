#include <QCoreApplication>
#include <iostream>
#include <string>
using namespace std;

void replaceSpace(char *str, int length){
    int blankNumber = 0;//空格的数量
    int oldstringLen;//记录原字符串的长度
    //首先遍历原字符串，找出字符串的长度以及其中的空格数量
    for (oldstringLen = 0; str[oldstringLen] != '\0'; oldstringLen++){

        if (str[oldstringLen] == ' ')
            blankNumber++;
    }

    //根据原字符串的长度和空格的数量我们可以求出最后新字符串的长度
    int newstringLen = oldstringLen + blankNumber * 2;//新字符串的长度
    if (newstringLen>length)
        return;
    str[newstringLen] = '\0';//此行很重要，因为原字符串最后一个字符为'\0'

    //设置两个指针point1和point2分别指向原字符串和新字符串的末尾位置
    int point1 = oldstringLen - 1, point2 = newstringLen - 1;//因为'\0'已经手工加到最后新串的最后一个字符，所以减1咯

    while (point1 >= 0 && point2>point1){//两指针相同时，跳出循环

        if (str[point1] == ' '){//如果point1指向为空格，那么从point2开始赋值“02%”
            str[point2--] = '0';
            str[point2--] = '2';
            str[point2--] = '%';

        }
        else    //如果point1指向内容不为空格，那么将内容赋值给point2指向的位置
            str[point2--] = str[point1];

        point1--;//不管是if还是else都要把point1前移,为了下一次的执行

    }


}

#define LENGTH_L 100
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


////////  进行空格替换
//    char *string=new char[5];//替换
//    string="aanndjdn";
//    char test[100]="we are happy.";
//    int n=strlen(test);
//    cout<<test<<endl;//we are happy.
//    replaceSpace(test,50);
//    cout<<test<<endl;
//    //===========字符串操作相关===============//
//    char p[20];//char数组进行输入和输出
//    cin>>p;
//    cout<<p<<endl;
//    char *p1=new char [10];//char指针进行输出
//    cin>>p1;
//    cout<<p1<<endl;
//    char p2;//只能保存一位并进行输出
//    cin>>p2;
//    cout<<p2<<endl;
    char p3[20];
    cin.get(p3,20);//可以进行空格键入   注意和gets函数用法区别
    cout<<p3<<endl;
     string str="sjdfasdf";//随便输入一串字符
     
     cout<<str<<endl;

    return a.exec();
}
