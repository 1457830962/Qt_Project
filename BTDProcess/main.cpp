#include <QCoreApplication>
#include "MeasuareMethod.h"
#include "cfileoperator.h"
#include "qfileoperator.h"
#include "offlinemeasuare.h"


#include<fstream>
#include <iostream>
#include <vector>
#include <string>
#include <QDir>
#include <QDebug>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MeasuareMethod method;


    ///----------------------------------------------- run muti img,write result in txtfile -------------------------------------------------------------//
    ofstream outfile;//定义文件变量
    outfile.open("/home/philip/Documents/Qt_Project/double40.txt",ios::out); //创建文件,OutPath为创建txt路径，用于保存处理结果

    //      vector<string> images_filenames;  //C++的方式 进行读取图片名
    //      CfileOperator::readDirectory("E:\\sc_exp\\fengdongTest0124", images_filenames, true);
    QFileInfoList return_list= QfileOperator::GetfileList("/home/philip/Documents/Qt_Project/bad");//Qt的方式  将返回的所有文件名列表赋给变量renturn_list;


    //将读取的文件名序列进行遍历
    for (size_t i = 0; i < return_list.size(); i++){

        bool bRct= method.GetKeyPt(return_list.at(i).absoluteFilePath().toStdString(),270,1,2,3);//正角度表示将图逆时针转
        //      outfile << return_list.at(i).absoluteFilePath().toStdString() <<" "<<method.x0 <<" "<<method.y0
        //                 <<" "<<method.RotorCode[0]<<method.RotorCode[1]<<method.RotorCode[2]<<"  "<<method.x1
        //                 <<" "<<method.y1<<" "<<method.RotorCode[3]<<method.RotorCode[4]<<method.RotorCode[5]<<" "<<method.y2<<"\n";
        ///////----------------------------CHECK THE PROGRAM-------------------------------------/////
        //      outfile << return_list.at(i).absoluteFilePath().toStdString() <<" "<<method.x0 <<" "<<method.y0
        //                 <<" "<<method.RotorCode[0]<<method.RotorCode[1]<<"  "<<method.x1
        //                 <<" "<<method.y1<<" "<<method.RotorCode[2]<<method.RotorCode[3]<<" "<<method.y2<<"======="<<method.y00 <<" "<<method.y01<<" "<<method.y02 <<" "<<method.y03<<" "<<method.y10 <<" "<<method.y11<<" "<<method.y12 <<" "<<method.y13<<"\n";
        ///////----------------------------Double output-------------------------------------------------//////////
        outfile << return_list.at(i).absoluteFilePath().toStdString() <<" "<<method.x0 <<" "<<method.y0
                <<" "<<method.RotorCode[0]<<method.RotorCode[1]<<"  "<<method.x1
               <<" "<<method.y1<<" "<<method.RotorCode[2]<<method.RotorCode[3]<<" "<<method.y2<<"\n";
        ///////---------------------------Single Output------------------------------------------------/////////
        //      outfile << return_list.at(i).absoluteFilePath().toStdString() <<" "<<method.x0 <<" "<<method.y0
        //                     <<" "<<method.RotorCode[0]<<method.RotorCode[1]<<"\n";
        ///////----------------------------Take the error----------------------------------------------//////////

        //      if(bRct==false)                 //判断并挑选出算法报错图片
        //      {
        //        outfile << return_list.at(i).absoluteFilePath().toStdString() <<" "<<method.x0 <<" "<<method.y0
        //                  <<" "<<method.RotorCode[0]<<method.RotorCode[1]<<method.RotorCode[2]
        //                  <<"\n";
        //      }

    }
    cout <<"----- processe is end! -----"<< endl;
    outfile.close();


    ///////--------------Single Img_Rotate_Anti-clock_90°----------------///
        //    QFileInfoList return_list= QfileOperator::GetfileList("C:\\Users\\Administrator\\Desktop\\bad");//将返回的所有文件名列表赋给变量renturn_list;
        //    method.IsShowImg=true;
        //    method.GetKeyPt("C:\\Users\\Administrator\\Desktop\\bad\\000_3_579.bmp",270,1,2,3);

    //////----------------------- run single img --------------------------------///
        //    method.IsShowImg = true;
        //    method.GetKeyPt("E:\\sc_exp\\double\\562.bmp",0,1,2,3);

   //////----------------------- Measuare offline -------------------------------///
        //    OfflineMeasuare offmeasure;
        //    offmeasure.ClassifyRawImgData("E:\\sc_exp\\test0127");
        //    offmeasure.ReadClassifyImgData("E:\\sc_exp\\test0127_1081转速_classify");

  //////----------------------- test fun -------------------------------///
        //    QString strcode;
        //    int collective_pitch;
        //    offmeasure.GetInfoByImgfileName("010_009_10.bmp",collective_pitch,strcode);
        return a.exec();
}
