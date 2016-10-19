#ifndef ROBOT_H
#define ROBOT_H
#include <QtCore>
#include <QObject>
#include<contrlCard.h>
#include<pasvcontrl.h>
#include<boundarydetection.h>
#include <eyecontrol.h>
#include<encoderacquisit.h>
#include <QTimer>
#include<QString>
class Robot :public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE Robot();
    ~Robot();
    Q_INVOKABLE QString showShoulAngle();
    Q_INVOKABLE QString showArmAngle();
    Q_INVOKABLE QString showArmORZ();
    Q_INVOKABLE QString showShoulORZ();
    Q_INVOKABLE void Reset2ORZ();//回到机器人的绝对零位置
    Q_INVOKABLE void Stop2ORZ();

    Q_INVOKABLE void getLeftCenter(QPoint pupilCenter);
    Q_INVOKABLE void getRightCenter(QPoint pupilCenter);

    Q_INVOKABLE void beginEyeContrl();//开启眼动运动
    Q_INVOKABLE void stopEyeContrl();//停止眼动运动
    Q_INVOKABLE void getCaliCenter();

private:
    double Comfort_Pos[2];//开始运动的初始位置，人觉得舒服的位置
    int leftCenterData[2];//
    int rightCenterData[2];//
    bool isGetCaliCenter;
    cv::Point leftCenter;
    cv::Point leftCaliCenter;
    cv::Point rightCenter;
    cv::Point rightCaliCenter;
    QMutex leftMutex;
    QMutex rightMutex;
    pasvContrl *pasvMode;
    contrlCard *ctrlCard;
    eyeControl *eyeLink;
    boundaryDetection *bDetect;
    encoderacquisit *jointAngle;
    double inputMotorCmd[2];
    int m_nTimerId;
    int m_eyeLinkTimerId;
    void timerEvent( QTimerEvent *event);
    void move2ORZ();
    void eyeControlMove(double eyeCmdData[2],double currentPos[2]);
};


#endif // ROBOT_H
