#ifndef BOUNDARYDETECTION_H
#define BOUNDARYDETECTION_H
#include <QThread>
#include<QMutex>
class boundaryDetection : public QThread
{
public:
    boundaryDetection();
    ~boundaryDetection();
     void run();
    bool* GetSwithData();
private:
     bool Travel_Switch[4];//光电行程开关的示数，0-ORG0，1-MEL0，2-ORG1，3-MEL1
     QMutex mutex;//加数据的读写锁
     bool m_stop;
     void getSensorData(bool Optoelc_Switch[4]);//获取四个光电开关状态

};

#endif // BOUNDARYDETECTION_H
