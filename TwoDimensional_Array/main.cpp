#include <QCoreApplication>
#include<csolution.h>
#include<iostream>
using namespace std;

#define HEIGHT 10
#define WIDTH 10

int target=88;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CSolution My_Solution;

    bool is_target=false;
    vector<vector<int> > array_1/*(HEIGHT,vector<int>(WIDTH,0))*/;//对二维容器进行初始化为0
    int array_2[HEIGHT][WIDTH];
    for(int i=0;i<HEIGHT;i++)
    {
        for(int j=0;j<WIDTH;j++)
        {
            array_1(i,vector<int>(j,1+i*j));
        }

    }


    is_target=My_Solution.Find_1(0,array_2);

    if(is_target)
        cout<<"Yes!"<<endl;
    else
        cout<<"No!"<<endl;



    return a.exec();
}
