#ifndef PASVCONTRL_H
#define PASVCONTRL_H
#include <QtCore>
#include <QThread>
#include<QQueue>
#include <QPair>
#include<QString>
#include <QWaitCondition>
#include <QMutex>
#include <QTimer>
class pasvContrl : public QObject
{
     Q_OBJECT
public:
    Q_INVOKABLE pasvContrl();
    //开始运动
    void init();
    Q_INVOKABLE void beginMove();
    void run();
    Q_INVOKABLE void stop();
    void addMovement(QString name,int*para);
private:

    bool isInInitPos;
    bool isMoveEnd;
    void OnPASVHermite(double PosArm,double PosShoul,double Time);
    //hermite插值算法-返回值为curtime时间点下电机的位置
    double PHermite(double foretime[2],//已知时间点
                        double forepos[2],//已知位置点
                        double forevel[2],//已知速度点
                        double t);////所求时间点
    //线程等待条件
    QWaitCondition cond;
    QMutex mutex;
    //定时器
    void timerEvent( QTimerEvent *event );
    int m_nTimerId;
    int timecount;
    double PASVHermite_time;
    //存储运动动作参数
    QPair<QString,int*>currentMove;
    QPair<QString,int*>moveParam;
    QQueue<QPair<QString,int*>>pasvMove;
};

#endif // PASVCONTRL_H
