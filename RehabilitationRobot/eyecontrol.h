#ifndef EYECONTROL_H
#define EYECONTROL_H
#include <QtCore>
#include <QObject>
#include<QQuickView>
#include<QQuickItem>
#include<QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
class eyeControl:public QObject
{
    Q_OBJECT
public:
    eyeControl();
    double* getCmdPos();
    void calculateCmdPos(cv::Point leftCenterData,
                         cv::Point rightCenterData,
                         cv::Point2d currentMotorAngles,
                         cv::Point leftCaliCenter,
                         cv::Point rightCaliCenter,
                         double inputMotorCmd[2]);
    void setCmdPos(double armCmdPos,double shoulCmdPos);
private:
    cv::Mat calculateWorkPlace();
    void eyeControlAlgorithm(const cv::Point leftPupilPos,
                   const cv::Point rightPupilPos,
                   const cv::Point leftCalibPupilPos,
                   const cv::Point rightCalibPupilPos,
                   const cv::Point2d currentMotorAngles,
                   cv::Point2d& outPutMotorAngles,
                   const cv::Mat surfaceXYZ,
                   const double motorU1Max,
                   const double motorU2Max,
                   const int controlMode
                   );
    double cmdPos[2];//电机的命令位置
    cv::Mat surfaceXYZ;
    QMutex mutex;//加数据的读写锁
};

#endif // EYECONTROL_H
