#ifndef DATATYPEDEFINE_H
#define DATATYPEDEFINE_H

#include <QString>
#include <QVector>
#include <QRectF>
#include <QPointF>
#include <QMap>

const int CPT_TATOL = 10;
const QMap<int ,int> map_cpt{
    {0,0},{2,1},{4,2},{6,3},{8,4},
    {9,5},{10,6},{11,7},{12,8},{13,9}};


typedef struct ST_ROTOR_IMG_DADA
{
    QString imgname;
    QString filepath;
    QPointF pt_pixel;  // 1/4 center point
    int collective_pitch;

//    int width;
//    int heigh;
//    int serialnum;
//    QVector<float> rect_pt;

}ST_ROTOR_IMG_DADA,*PST_ROTOR_IMG_DADA;

typedef struct ST_SINGLE_ROTOR_DATA_VECTOR
{
    QVector<ST_ROTOR_IMG_DADA> vec[CPT_TATOL];
    QString strcode;

}ST_SINGLE_ROTOR_DATA_VECTOR,*PST_SINGLE_ROTOR_DATA_VECTOR;

#endif // DATATYPEDEFINE_H
