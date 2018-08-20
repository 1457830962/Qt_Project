#include "qfileoperator.h"


QfileOperator::QfileOperator()
{
}

//返回传入文件夹中所有文件名列表
QFileInfoList QfileOperator::GetfileList(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);//子文件夹中文件

    for (int i = 0; i != folder_list.size();i++ )
    {
        QString name = folder_list.at(i).absoluteFilePath();//取到子文件夹中文件并取到其绝对路径赋值
        QFileInfoList child_file_list = GetfileList(name);   //相当于迭代调用Getfilelist函数
        file_list.append(child_file_list);
    }
    return file_list;
}

void QfileOperator::CreateMutiDirs(QString path, QStringList code_list)
{
    QDir dir(path);
    for (int i = 0 ; i<code_list.size(); i++)
    {
        dir.mkdir(code_list.at(i));
    }
}
