#include "encoderacquisit.h"
#include<contrlCard.h>
encoderacquisit::encoderacquisit()
{
    angle[0]=0;
    angle[1]=0;
    m_stop=false;
}
encoderacquisit::~encoderacquisit()
{
    qDebug()<<"encoderacquisit destroyed.";
    if(isRunning())
    {
        m_stop = true;//暂停下载
        quit();
        wait();
    }
}
void encoderacquisit::getEncoderData(double EncoderData[2])
{
        int ret=0;
        double raw_arm=0;
        double  raw_shoulder=0;
        ret=APS_get_position_f(elbowAxisId, &raw_arm);
        ret=APS_get_position_f(shoudlerAxisId, &raw_shoulder);
        EncoderData[0]=raw_shoulder*Unit_Convert;
        EncoderData[1]=raw_arm*Unit_Convert;
        /*if(ret)
            qDebug()<<"GetEncoderData failed!  ERROCode is !"<<ret;*/
}
void encoderacquisit::run()
{
    qDebug()<<"encoderAcquisit Thread begin!";
    forever{
        if(m_stop)
            break;
        mutex.lock();
        getEncoderData(angle);
        mutex.unlock();
    }
    qDebug()<<"encoderAcquisit Thread end!";
}
double* encoderacquisit::getAngle()
{
    mutex.lock();
    double *output=angle;
    mutex.unlock();
    return output;
}
