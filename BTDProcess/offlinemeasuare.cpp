#include "offlinemeasuare.h"
#include "qfileoperator.h"
#include "MeasuareMethod.h"
#include <QDir>
#include <QDebug>
#include <fstream>

using namespace std;

OfflineMeasuare::OfflineMeasuare()
{

}

OfflineMeasuare::~OfflineMeasuare()
{
    if (pst_Rotor != NULL)
    {
        delete [] pst_Rotor;
    }
}

bool OfflineMeasuare::ClassifyRawImgData(QString path)
{
    MeasuareMethod method;

    QString rawdata_path(path);
    QDir dir(rawdata_path);
    if (!dir.exists())
    {
        qDebug() << "Dir not exsits!";
        return false;
    }
    ///Get all raw img path
    QFileInfoList file_list = QfileOperator::GetfileList(rawdata_path);

    ///Create relative dirs
    QString classifydirname = dir.dirName()+"_classify";

    dir.cdUp();
    dir.mkdir(classifydirname);
    dir.cd(classifydirname);
    qDebug() << "Code parent Dir is: "<<dir.absolutePath();

    //QfileOperator::CreateMutiDirs(dir.absolutePath(),QStringList({"000","111"}));


    for (int i=0;i<file_list.size();i++)
    {
        QString img_name = file_list.at(i).fileName();
        qDebug() << "Current img file name is: "<<img_name;

        method.GetKeyPt(file_list.at(i).absoluteFilePath().toStdString(),90,1,2,1);
        QString strcode;
        for (int i=0;i<3;i++)
        {
            if (method.RotorCode[i] == false)
                strcode += "0";
            else
                strcode += "1";
        }
        QDir codedir(dir.absolutePath()+"/"+strcode);
        if (!codedir.exists())
            dir.mkdir(strcode);

        QString src = file_list.at(i).absoluteFilePath();
        img_name.replace(0,3,strcode);
        QString dst = codedir.absolutePath()+"/"+img_name;

        qDebug() << src;
        qDebug() << dst;

        QFile::copy(src,dst);
    }

    qDebug() << "------Classify img data done!------";

    return true;
}

bool OfflineMeasuare::ReadClassifyImgData(QString path)
{
    QDir txtdir(path);
    QString txtpath = txtdir.absolutePath()+"/result.txt";
    ofstream outfile;//定义文件变量
    outfile.open(txtpath.toStdString(),ios::out); //创建文件,OutPath为创建txt路径

    QDir dir(path);
    QStringList dirlist = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);

    const int rotor_num = dirlist.size();

    if (rotor_num <= 0)
        return false;

    pst_Rotor = new ST_SINGLE_ROTOR_DATA_VECTOR[rotor_num];

    for (int i = 0;i<rotor_num;i++)
    {
        qDebug() << dirlist.at(i);
        pst_Rotor[i].strcode = dirlist.at(i);
    }

    MeasuareMethod method;

    for (int i =0; i<dirlist.size();i++)
    {
        dir.cd(dirlist.at(i));

        QStringList nameFilters;
        nameFilters << "*.bmp";

        QStringList imglist = dir.entryList(nameFilters,QDir::Files | QDir::Readable,QDir::Name);
        for (int j =0; j<imglist.size();j++)
        {
            qDebug() << imglist.at(j);
            QString strcode;
            int collective_pitch;
            GetInfoByImgfileName(imglist.at(j),collective_pitch,strcode);
            if (strcode == pst_Rotor[i].strcode)
            {
                ST_ROTOR_IMG_DADA st_imgdata;
                st_imgdata.filepath = dir.absolutePath()+"/"+imglist.at(i);
                method.GetKeyPt(st_imgdata.filepath.toStdString(),90,1,2,1);
                st_imgdata.pt_pixel =QPointF(method.x0,method.y0);
                st_imgdata.collective_pitch = collective_pitch;
                pst_Rotor[i].vec[map_cpt.find(collective_pitch).value()].push_back(st_imgdata);
                outfile <<method.RotorCode[0]<<method.RotorCode[1]<<method.RotorCode[2] <<" "<<st_imgdata.collective_pitch<<" "<<method.x0 <<" "<<method.y0
                                <<" "<< st_imgdata.filepath.toStdString()
                                <<"\n";
            }
        }
        dir.cdUp();
    }

    for (int i = 0;i<rotor_num;i++)
    {
        qDebug() <<"pst_Rotor[i].vec[0].size() = "<<pst_Rotor[i].vec[0].size();
    }

    qDebug() <<"------------- done!-----------" ;
    outfile.close();
    return true;
}

void OfflineMeasuare::GetInfoByImgfileName(QString imgfilename,int& collective_pitch,QString& strcode)
{
    QString imgname = imgfilename.section('.',0,0);
    //qDebug() << "imgname"<<imgname;

    strcode = imgname.section('_',0,0);
    //qDebug() <<"strcode:"<< strcode;

    collective_pitch = imgname.section('_',1,1).toInt();//zongju
    //qDebug() <<"collective_pitch:"<< collective_pitch;
}
