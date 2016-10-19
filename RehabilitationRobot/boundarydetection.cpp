#include "boundarydetection.h"
#include "APS168.h"
#include <QDebug>
#define __MAX_DI_CH (24)
boundaryDetection::boundaryDetection()
{
    for(int i=0;i<4;i++)
            Travel_Switch[i]=0;
    m_stop=false;
}
boundaryDetection::~boundaryDetection()
{
    qDebug()<<"boundaryDetection destroyed.";
    if(isRunning())
    {
        m_stop = true;//暂停下载
        quit();
        wait();
    }
}
void boundaryDetection::run()
{
    qDebug()<<"BoundaryDetection Thread begin!";
    forever
    {
        if(m_stop)
            break;

        mutex.lock();
        getSensorData(Travel_Switch);
        mutex.unlock();

    }
    qDebug()<<"boundaryDetection Thread ended.";
}
void boundaryDetection::getSensorData(bool Optoelc_Switch[4])
{
    I32 DI_Group = 0; // If DI channel less than 32
    I32 DI_Data = 0; // Di data
    I32 di_ch[__MAX_DI_CH];
    I32 returnCode = 0; // Function return code
    returnCode = APS_read_d_input(0, DI_Group, &DI_Data);
    for (int i = 0; i < __MAX_DI_CH; i++)
        di_ch[i] = ((DI_Data >> i) & 1);

    Optoelc_Switch[0]=di_ch[16];//0号电机ORG信号
    Optoelc_Switch[1]=di_ch[17];//0号电机MEL信号

    Optoelc_Switch[2]=di_ch[18];//1号电机ORG信号
    Optoelc_Switch[3]=di_ch[19];//1号电机MEL信号
}
 bool* boundaryDetection::GetSwithData()
 {
     mutex.lock();
     bool *output=Travel_Switch;
     mutex.unlock();
     return output;
 }
