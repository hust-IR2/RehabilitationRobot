#include "pasvcontrl.h"
#include "APS168.h"
#include <QDebug>
#define PASVTimer 0.04
#define shoudlerAxisId 0
#define elbowAxisId 1
#define Unit_Convert  0.009
const double Comfort_Pos[2]={12.5,1.6};//开始运动的初始位置，人觉得舒服的位置
pasvContrl::pasvContrl()
{
    //初始化动作队列
    init();
    isInInitPos=true;
    isMoveEnd=false;
    timecount=0;
    PASVHermite_time=0;
    qDebug()<<"pasvContrl construct done!";
}
void pasvContrl::addMovement(QString name,int*para)
{
    moveParam.first=name;
    moveParam.second=para;
    pasvMove.enqueue(moveParam);
}
void pasvContrl::init()
{
    int existMove[4][2]={{71,20},{51,66},{57,41},{62,35}};
    addMovement("Bend Elbow",existMove[0]);
    addMovement("Touch Shoulder",existMove[1]);
    addMovement("Touch Mouth",existMove[2]);
    addMovement("Touch Head",existMove[3]);
}
void pasvContrl::run()
{
    currentMove=pasvMove.head();
    int *currentParam=currentMove.second;

    forever
    {
        if(!isInInitPos||isMoveEnd)
            break;
        cond.wait(&mutex);
        OnPASVHermite(currentParam[0],currentParam[2],8);
    }
}
void pasvContrl::beginMove()
{
    //this->start();//开启线程
    qDebug()<<"passive motion begin";
    m_nTimerId = startTimer(PASVTimer*1000);//设置定时器
    currentMove=pasvMove.head();
    isMoveEnd=false;
    timecount=0;
}
void pasvContrl::timerEvent( QTimerEvent *event )
{
    qDebug()<<"isInInitPos is:"<<isInInitPos;
    qDebug()<<"isMoveEnd is:"<<isMoveEnd;
    if(!isInInitPos||isMoveEnd)
        stop();
    int *currentParam=currentMove.second;
    qDebug()<<"Arm target pos is:"<<currentParam[0];
    qDebug()<<"Shoulder target pos is:"<<currentParam[1];
    OnPASVHermite(71,20,8);

}
void pasvContrl::stop()
{
    qDebug()<<"passive motion end";
    if (m_nTimerId != 0 )
          killTimer(m_nTimerId);
    if(!pasvMove.empty()&&isMoveEnd)
        pasvMove.dequeue();
}
void pasvContrl::OnPASVHermite(double PosArm,double PosShoul,double Time)
{
        double time_forwrd[2]={0,Time};//回到初始位置的时间*-++*+++
        double time_back[2]={Time+2,Time*2+2};//回到初始位置的时间

        double vel_forwrd[2]={0,0};//回到初始位置的始末速度
        double vel_back[2]={0,0};//回到初始位置的始末速度

        double armpos_forwrd[2]={Comfort_Pos[1],PosArm};
        double armpos_back[2]={PosArm,Comfort_Pos[1]};

        double shoulpos_forwrd[2]={Comfort_Pos[0],PosShoul};
        double shoulpos_back[2]={PosShoul,Comfort_Pos[0]};
        //被动模式命令位置
        double arm_motor_cmd=0;
        double shoul_motor_cmd=0;
        PASVHermite_time=timecount*PASVTimer;
        if (PASVHermite_time<=Time)
        {
            arm_motor_cmd=PHermite(time_forwrd,armpos_forwrd,vel_forwrd,PASVHermite_time);
            APS_absolute_move(elbowAxisId,arm_motor_cmd/Unit_Convert,15/Unit_Convert);


            shoul_motor_cmd=PHermite(time_forwrd,shoulpos_forwrd,vel_forwrd,PASVHermite_time);
            APS_absolute_move(shoudlerAxisId,shoul_motor_cmd/Unit_Convert,15/Unit_Convert);

        }
        else if((PASVHermite_time<=(Time*2+2))&&(PASVHermite_time>=(Time+2)))
        {
            arm_motor_cmd=PHermite(time_back,armpos_back,vel_back,PASVHermite_time);
            APS_absolute_move(elbowAxisId,arm_motor_cmd/Unit_Convert,15/Unit_Convert);


            shoul_motor_cmd=PHermite(time_back,shoulpos_back,vel_back,PASVHermite_time);
            APS_absolute_move(shoudlerAxisId,shoul_motor_cmd/Unit_Convert,15/Unit_Convert);

        }
        else if(PASVHermite_time>=(Time*2+2))
        {
            isMoveEnd=true;
            qDebug()<<"Motion Done!";
        }
        qDebug()<< "Arm cmd pos is:" << arm_motor_cmd;
        qDebug()<< "Shoulder cmd pos is:" << shoul_motor_cmd;
        timecount++;
}
double pasvContrl::PHermite(double foretime[2],double forepos[2],double forevel[2],double t)
{
    double Houtput=0;
    double a[2]={0};
    double b[2]={0};
    a[0]=(1-2*(t-foretime[0])/(foretime[0]-foretime[1]))*pow((t-foretime[1])/(foretime[0]-foretime[1]),2);
    a[1]=(1-2*(t-foretime[1])/(foretime[1]-foretime[0]))*pow((t-foretime[0])/(foretime[1]-foretime[0]),2);
    b[0]=(t-foretime[0])*pow((t-foretime[1])/(foretime[0]-foretime[1]),2);
    b[1]=(t-foretime[1])*pow((t-foretime[0])/(foretime[1]-foretime[0]),2);
    Houtput=a[0]*forepos[0]+a[1]*forepos[1]+b[0]*forevel[0]+b[1]*forevel[1];
    return Houtput;
}
