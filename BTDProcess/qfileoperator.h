#ifndef QFILEOPERATOR_H
#define QFILEOPERATOR_H
#include <QDir>
#include <QString>

class QfileOperator
{
private:
    QfileOperator();
public:
    static QFileInfoList GetfileList(QString path);
    static void CreateMutiDirs(QString path,QStringList code_list);
};

#endif // QFILEOPERATOR_H
