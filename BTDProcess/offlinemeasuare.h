#ifndef OFFLINEMEASUARE_H
#define OFFLINEMEASUARE_H

#include "datatypedefine.h"
#include <QMap>
#include <QString>

class OfflineMeasuare
{
public:
    PST_SINGLE_ROTOR_DATA_VECTOR pst_Rotor;
public:
    OfflineMeasuare();
    ~OfflineMeasuare();
    bool ClassifyRawImgData(QString path);
    bool ReadClassifyImgData(QString path);
    void GetInfoByImgfileName(QString imgfilename,int& collective_pitch,QString& strcode);

};

#endif // OFFLINEMEASUARE_H
