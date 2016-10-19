#include "robot.h"
Robot::Robot()
{
    ctrlCard=NULL;
    ctrlCard=new contrlCard;
    bDetect=NULL;
    bDetect=new boundaryDetection;
    eyeLink=NULL;
    eyeLink=new eyeControl;

    jointAngle=NULL;
    jointAngle=new encoderacquisit;
    m_eyeLinkTimerId=0;
    m_nTimerId=0;
    isGetCaliCenter=false;
    for(int i=0;i<2;i++)
    {
        Comfort_Pos[i]=0;
        leftCenterData[i]=0;
        rightCenterData[i]=0;
        inputMotorCmd[i]=0;
    }
    leftCenter.x=0;
    leftCenter.y=0;
    rightCenter.x=0;
    rightCenter.y=0;
    ctrlCard->Initial();
    bDetect->start();
    jointAngle->start();
}
Robot::~Robot()
{
    qDebug()<<"Robot Stoped!";
    if(ctrlCard!=NULL)
        delete ctrlCard;
    if(bDetect!=NULL)
        delete bDetect;
    if(eyeLink!=NULL)
        delete eyeLink;
    if(jointAngle!=NULL)
        delete jointAngle;
}
void Robot::move2ORZ()
{
    //判断0号电机是否在零位，如果不是则回零位运动开始，如果是则停止运动
        bool *RobotORZ=bDetect->GetSwithData();

        if (RobotORZ[0]!=true)
        {
            APS_vel(shoudlerAxisId,1,5/Unit_Convert,0);
        }
        else
        {
            APS_stop_move(shoudlerAxisId);
        }
        //判断1号电机是否在零位，如果不是则回零位运动开始，如果是则停止运动
        if (RobotORZ[2]!=true)
        {
            APS_vel(elbowAxisId,1,5/Unit_Convert,0);
        }
        else
        {
            APS_stop_move(elbowAxisId);
        }

        if ((RobotORZ[0]==true)&&(RobotORZ[2]==true))
        {
            qDebug()<<"Reset2ORZ completed!";
            ctrlCard->ServeTheMotor(OFF);
            ctrlCard->SetClutch(OFF);
            ctrlCard->SetParamZero();
            if (m_nTimerId != 0 )
            {
                killTimer(m_nTimerId);
                m_nTimerId=0;
            }
        }
}
void Robot::Reset2ORZ()
{
    qDebug()<<"Reset2ORZ begin";
    ctrlCard->ServeTheMotor(ON);
    m_nTimerId = startTimer(100);//设置定时器
    ctrlCard->SetClutch(ON);
}
void Robot::Stop2ORZ()
{
    qDebug()<<"Reset2ORZ Stoped!";
    ctrlCard->ServeTheMotor(OFF);
    ctrlCard->SetClutch(OFF);
    if (m_nTimerId != 0 )
    {
        killTimer(m_nTimerId);
        m_nTimerId=0;
    }
}
void Robot::timerEvent( QTimerEvent *event)
{
    int eventID=event->timerId();
    if(eventID==m_nTimerId)
    {
        qDebug()<<"Reset Timer ID is: "<<eventID;
        move2ORZ();
    }
    if(eventID==m_eyeLinkTimerId)
    {
        if(isGetCaliCenter)
        {
            qDebug()<<"EyeControl Timer ID is: "<<eventID;
            double inputCmd[2]={0};
            double *currentPos=jointAngle->getAngle();
            qDebug()<<"EyeContrl CurrentAngle is : "<<currentPos[0]<<","<<currentPos[1];
            cv::Point2d currentMotorAngles;
            /*currentMotorAngles.x=currentPos[0];
            currentMotorAngles.y=currentPos[1];*/
            currentMotorAngles.x=inputMotorCmd[0];
            currentMotorAngles.y=inputMotorCmd[1];
            eyeLink->calculateCmdPos(leftCenter,rightCenter,currentMotorAngles,leftCaliCenter,rightCaliCenter,inputCmd);

            qDebug()<<"EyeContrl CmdAngle is : "<<inputCmd[0]<<","<<inputCmd[1];
            inputMotorCmd[0]=inputCmd[0];
            inputMotorCmd[1]=inputCmd[1];

            eyeControlMove(inputMotorCmd,currentPos);
        }
        else
        {
             qDebug()<<"Please GetCaliCenter!";
             stopEyeContrl();
        }

    }
}
void Robot::beginEyeContrl()
{
    //eyeLink=new eyeControl;
    qDebug()<<"EyeContrl begin";
    ctrlCard->ServeTheMotor(ON);
    ctrlCard->SetClutch(ON);
    m_eyeLinkTimerId = startTimer(500);//设置定时器
}
void Robot::stopEyeContrl()
{
    qDebug()<<"EyeContrl Stoped!";
    ctrlCard->ServeTheMotor(OFF);
    ctrlCard->SetClutch(OFF);
    if (m_eyeLinkTimerId != 0 )
    {
        killTimer(m_eyeLinkTimerId);
        m_eyeLinkTimerId=0;
    }
}
void Robot::eyeControlMove(double eyeCmdData[], double currentPos[])
{
    int maxAngle[2]={60,55};
    double moveSpeed[2]={0};
    int AxisID[2]={shoudlerAxisId,elbowAxisId};
    for(int i=0;i<2;i++){
        if(eyeCmdData[i]>maxAngle[i])
            eyeCmdData[i]=maxAngle[i];
        else if(eyeCmdData[i]<0)
            eyeCmdData[i]=0;

        /*if(eyeCmdData[1]>55)
            eyeCmdData[1]=55;
        else if(eyeCmdData[1]<0)
            eyeCmdData[1]=0;*/

        moveSpeed[i]=(eyeCmdData[i]-currentPos[i])*0.3;
        if(currentPos[i]>=maxAngle[i])moveSpeed[i]=0;

        if(moveSpeed[i]>=10)moveSpeed[i]=10;
        else if(moveSpeed[i]<=-10)moveSpeed[i]=-10;



        qDebug()<<"moveSpeed is"<<moveSpeed[i];
        if(moveSpeed[i]>=0)
            APS_vel(AxisID[i],0,moveSpeed[i]/Unit_Convert,0);
        else
            APS_vel(AxisID[i],1,-moveSpeed[i]/Unit_Convert,0);
    }

}
QString Robot::showShoulAngle()
{
     double *currentPos=jointAngle->getAngle();
     int output=static_cast<int>(currentPos[0]);
     QString Pos;
     Pos.setNum(output);
     return Pos;
}
QString Robot::showArmAngle()
{
    double *currentPos=jointAngle->getAngle();
     int output=static_cast<int>(currentPos[1]);
    QString Pos;
    Pos.setNum(output);
    return Pos;
}
QString Robot::showArmORZ()
{
    bool *RobotORZ=bDetect->GetSwithData();
    QString ORZ;
    if(RobotORZ[2])
        ORZ="Arrived";
    else
        ORZ="Free";
    return ORZ;
}
QString Robot::showShoulORZ()
{
    bool *RobotORZ=bDetect->GetSwithData();
    QString ORZ;
    if(RobotORZ[0])
        ORZ="Arrived";
    else
        ORZ="Free";
    return ORZ;
}
void Robot::getLeftCenter(QPoint pupilCenter)
{
//    qDebug()<<pupilCenter;
    leftMutex.lock();
    leftCenter.x=pupilCenter.x();
    leftCenter.y=pupilCenter.y();
    leftMutex.unlock();
}
void Robot::getRightCenter(QPoint pupilCenter)
{
//    qDebug()<<pupilCenter;
    rightMutex.lock();
    rightCenter.x=pupilCenter.x();
    rightCenter.y=pupilCenter.y();
    rightMutex.unlock();
}
void Robot::getCaliCenter()
{
    leftCaliCenter=leftCenter;
    rightCaliCenter=rightCenter;
    isGetCaliCenter=true;
    qDebug()<<"leftEyeCaliCenter: "<<leftCaliCenter.x<<", "<<leftCaliCenter.y;
    qDebug()<<"rightEyeCaliCenter: "<<rightCaliCenter.x<<", "<<rightCaliCenter.y;
}
