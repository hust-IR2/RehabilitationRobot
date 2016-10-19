#include "eyecontrol.h"
using namespace cv;
eyeControl::eyeControl()
{
    cmdPos[0]=0;
    cmdPos[1]=0;
    surfaceXYZ=calculateWorkPlace();//构造的时候算好操作空间
}
double* eyeControl::getCmdPos()
{
     mutex.lock();
     double *output=cmdPos;
     mutex.unlock();
     return output;
}
void eyeControl::setCmdPos(double shoulCmdPos,double armCmdPos)
{

    if(shoulCmdPos>70)
        shoulCmdPos=70;
    else if(shoulCmdPos<0)
        shoulCmdPos=0;
    if(armCmdPos>70)
        armCmdPos=70;
    else if(armCmdPos<0)
        armCmdPos=0;
     mutex.lock();
     cmdPos[0]=shoulCmdPos;
     cmdPos[1]=armCmdPos;
     mutex.unlock();
}
void eyeControl::calculateCmdPos(Point leftCenterData,
                                 Point rightCenterData,
                                 Point2d currentMotorAngles,
                                 Point leftCaliCenter,
                                 Point rightCaliCenter,
                                 double inputMotorCmd[])
{
    int controlMode =0;//0:feedback control , 1: feedforward control
    Point2d outPutMotorAngles;
    eyeControlAlgorithm(leftCenterData,
                        rightCenterData,
                        leftCaliCenter,
                        rightCaliCenter,
                        currentMotorAngles,
                        outPutMotorAngles,
                        surfaceXYZ,
                        60.0,
                        55.0,
                        controlMode);
    qDebug()<<"outputtMotorAngles is "<<outPutMotorAngles.x<<","<<outPutMotorAngles.y;
    inputMotorCmd[0]=outPutMotorAngles.x;
    inputMotorCmd[1]=outPutMotorAngles.y;
}
cv::Mat eyeControl::calculateWorkPlace()
{
    //declare the homo transform matrix
        double T0sInit[4][4]={{-1,0,0,0.468},{0,0,-1,0},{0,-1,0,0.23},{ 0,0,0,1}};
        Mat T0s(4,4,CV_64FC1,T0sInit);
        Mat Ts1(4,4,CV_64FC1,Scalar(0));
        Mat T12(4,4,CV_64FC1,Scalar(0));
        Mat T23(4,4,CV_64FC1,Scalar(0));
        Mat T34(4,4,CV_64FC1,Scalar(0));
        Mat T45(4,4,CV_64FC1,Scalar(0));
        Mat T05(4,4,CV_64FC1,Scalar(0));

    //homo transform matrix invariant param init
        Ts1.at<double>(0,0)=1;
        Ts1.at<double>(3,3)=1;

        T12.at<double>(1,1)=1;
        T12.at<double>(3,3)=1;


        T23.at<double>(2,2)=1;
        T23.at<double>(3,3)=1;

        T34.at<double>(0,3)=-307; //unit: mm
        T34.at<double>(2,2)=1;
        T34.at<double>(3,3)=1;

        T45.at<double>(0,0)=1;
        T45.at<double>(0,3)=-270.06; //unit:mm
        T45.at<double>(3,3)=1;


    //declare the motor control angle and param
        double thetaHatInit[5] = {0,-7.44,0,26.5885,-21.4734}; // unit:degree
        double etaInit[5][2] = {{1,0},{0.88,0},{0,1},{0,1.3214},{0,0.6607}};
        const Mat thetaHat(5,1,CV_64FC1,thetaHatInit);
        const Mat eta(5,2,CV_64FC1,etaInit);
        Mat motorU(2,1,CV_64FC1,Scalar(0));
        Mat theta(6,1,CV_64FC1,Scalar(0));

        double motorU1Max=60;   //unit :degree
        double motorU2Max=55;

        unsigned int controlSteps=200;

        Mat surfaceXYZ;
        surfaceXYZ = Mat::zeros(controlSteps, controlSteps, CV_64FC3);
        for(unsigned int i=0;i<controlSteps;i++)
            for(unsigned int j=0;j<controlSteps;j++)
            {
                motorU.at<double>(0) = motorU1Max*i/controlSteps;
                motorU.at<double>(1) = motorU2Max*j/controlSteps;

                theta= thetaHat + eta*motorU;

                Ts1.at<double>(1,1) =  cos(theta.at<double>(0)/360*6.28);
                Ts1.at<double>(1,2) =  -sin(theta.at<double>(0)/360*6.28);
                Ts1.at<double>(2,1) =  sin(theta.at<double>(0)/360*6.28);
                Ts1.at<double>(2,2) =  cos(theta.at<double>(0)/360*6.28);

                T12.at<double>(0,0) =  cos(theta.at<double>(1)/360*6.28);
                T12.at<double>(0,2) =  sin(theta.at<double>(1)/360*6.28);
                T12.at<double>(2,0) =  -sin(theta.at<double>(1)/360*6.28);
                T12.at<double>(2,2) =  cos(theta.at<double>(1)/360*6.28);

                T23.at<double>(0,0) =  cos(theta.at<double>(2)/360*6.28);
                T23.at<double>(0,1) =  -sin(theta.at<double>(2)/360*6.28);
                T23.at<double>(1,0) =  sin(theta.at<double>(2)/360*6.28);
                T23.at<double>(1,1) =  cos(theta.at<double>(2)/360*6.28);

                T34.at<double>(0,0) =  cos(theta.at<double>(3)/360*6.28);
                T34.at<double>(0,1) =  -sin(theta.at<double>(3)/360*6.28);
                T34.at<double>(1,0) =  sin(theta.at<double>(3)/360*6.28);
                T34.at<double>(1,1) =  cos(theta.at<double>(3)/360*6.28);

                T45.at<double>(1,1) =  cos(theta.at<double>(4)/360*6.28);
                T45.at<double>(1,2) =  sin(theta.at<double>(4)/360*6.28);
                T45.at<double>(2,1) =  -sin(theta.at<double>(4)/360*6.28);
                T45.at<double>(2,2) =  cos(theta.at<double>(4)/360*6.28);

                T05 = T0s*Ts1*T12*T23*T34*T45;

                surfaceXYZ.at<Vec3d>(i,j)[0]=T05.at<double>(0,3);
                surfaceXYZ.at<Vec3d>(i,j)[1]=T05.at<double>(1,3);
                surfaceXYZ.at<Vec3d>(i,j)[2]=T05.at<double>(2,3);
            }
        return surfaceXYZ;
}
void eyeControl::eyeControlAlgorithm(const cv::Point leftPupilPos,
               const cv::Point rightPupilPos,
               const cv::Point leftCalibPupilPos,
               const cv::Point rightCalibPupilPos,
               const cv::Point2d currentMotorAngles,
               cv::Point2d& outPutMotorAngles,
               const cv::Mat surfaceXYZ,
               const double motorU1Max,
               const double motorU2Max,
               const int controlMode
               )
{


    Point2d BiMeanDirection;

    Point3d controlDirection;

    Point pupilDirectionL, pupilDirectionR ,depthDireciton;
    pupilDirectionL = leftPupilPos-leftCalibPupilPos;
    pupilDirectionR = rightPupilPos-rightCalibPupilPos;

    BiMeanDirection = (pupilDirectionL+ pupilDirectionR)/2;

//    double deptIndex = 2 ;

    double controlAmp = 0.8 ;

    int controlSteps = surfaceXYZ.rows;

    int i,j;

    int minDisI=1000;
    int minDisJ=1000;
    double minDistance=10000;

//    int depthIndexAdaptivReduce = 5 ;



    /*feedback control: head pose to the object, use eye to guide the robot to the object,
     * as which means that the robot move just towards the  opposite direction of the
     * pupilDirection
     */

    if (0 != controlMode)
    {
        controlDirection.y = -BiMeanDirection.x;
        controlDirection.x = -BiMeanDirection.y;
//        controlDirection.z = -deptIndex*(pupilDirectionL.x - pupilDirectionR.x);
        controlDirection.z=0;
    }

    /*feedforward control: consider the movement of the eye as a direction of exactly the
     * movement of the robot
     */
    else
    {
        controlDirection.y = BiMeanDirection.x;
        controlDirection.x = BiMeanDirection.y;
//        controlDirection.z = deptIndex*(pupilDirectionL.x - pupilDirectionR.x);
        controlDirection.z=0;
        qDebug()<<"pupilDirectionL x&y"<<pupilDirectionL.x<<" &"<<pupilDirectionL.y;
        qDebug()<<"pupilDirectionR x&y"<<pupilDirectionR.x<<" &"<<pupilDirectionR.y;
        qDebug()<<"BimeanDireciton x&y"<<BiMeanDirection.x<<" &"<<BiMeanDirection.y;
        qDebug()<<"controlDirection x&y&z"<<controlDirection.x<<" &"<<controlDirection.y<<" &"<<controlDirection.z;
        qDebug()<<"controlMode  :"<<"feedforward";
    }


    //current point in the workspace index
    controlDirection = controlAmp*controlDirection;

    if (norm(controlDirection) >= 20) {
        controlDirection = controlDirection / norm(controlDirection) * 20;
    }

    i = currentMotorAngles.x/motorU1Max * controlSteps;
    j = currentMotorAngles.y/motorU2Max * controlSteps;

    /*earch the nearest point in the robot workspace in a neighr area of 1/20 of the total workspace to the next controled point
    init the current point as the nearest point*/

    qDebug()<<"init Dist "<<minDistance;
    //qDebug()<<"surfaceXYZ dims h w"<<surfaceXYZ.size().height<<"*"<<surfaceXYZ.size().width;


//    while(depthIndexAdaptivReduce>0)
//    {

    for (int tempI = (((i-controlSteps/25)<0)? 0:(i-controlSteps/25));
          tempI<(((i+controlSteps/25)<controlSteps) ? (i+controlSteps/25): controlSteps);tempI++)
    {
        for (int tempJ = (((j-controlSteps/25)<0) ? 0: (j-controlSteps/25));
             tempJ<(((j+controlSteps/25)<controlSteps) ? (j+controlSteps/25): controlSteps);tempJ++ )

        {

            if(tempI==i&&tempJ==j)
                continue;

            if ( norm( surfaceXYZ.at<Point3d>(i,j)+controlDirection-surfaceXYZ.at<Point3d>(tempI,tempJ)) < minDistance )
            {
                 minDisI = tempI;
                 minDisJ = tempJ;
                 minDistance = norm( surfaceXYZ.at<Point3d>(i,j)+controlDirection-surfaceXYZ.at<Point3d>(tempI,tempJ));

            }
        }
    }

//        if ((minDisI == i) && (minDisJ == j))
//        {
//            controlDirection.z = controlDirection.z * 0.5;
//            minDistance = norm(controlDirection);
//            qDebug()<<"adapted init Dist "<<minDistance;
//        }

//        depthIndexAdaptivReduce--;
//        qDebug()<<"depthIndexAdaptivReduce "<<depthIndexAdaptivReduce;


//    }


    //if the choosed point is too close to the current point ,then choose the best point along the choosed direction appropratively
    //whose length is larger




    if (norm( surfaceXYZ.at<Point3d>(i,j)-surfaceXYZ.at<Point3d>(minDisI,minDisJ))<50)
    {
        cv::Point3d  tempVerc=
                (surfaceXYZ.at<Point3d>(minDisI,minDisJ) - surfaceXYZ.at<Point3d>(i,j))
                /
                (norm((surfaceXYZ.at<Point3d>(minDisI,minDisJ) -surfaceXYZ.at<Point3d>(i,j))) + 0.0001)
                *
                30;
        double tempMinDist = 1000;
        int temMinI,temMinJ;

        for (int tempI = (((minDisI-controlSteps/50)<0)? 0:(minDisI-controlSteps/50));
              tempI<(((minDisI+controlSteps/50)<controlSteps) ? (minDisI+controlSteps/50): controlSteps);tempI++)
        {
            for (int tempJ = (((minDisJ-controlSteps/50)<0) ? 0: (minDisJ-controlSteps/50));
                 tempJ<(((minDisJ+controlSteps/50)<controlSteps) ? (minDisJ+controlSteps/50): controlSteps);tempJ++ )

            {

                if ( norm( surfaceXYZ.at<Point3d>(minDisI,minDisJ)+tempVerc-surfaceXYZ.at<Point3d>(tempI,tempJ)) < tempMinDist )
                {
                     temMinI = tempI;
                     temMinJ= tempJ;
                     tempMinDist = norm( surfaceXYZ.at<Point3d>(minDisI,minDisJ)+tempVerc-surfaceXYZ.at<Point3d>(tempI,tempJ));
                }
            }
        }
        minDisI=temMinI;
        minDisJ=temMinJ;
    }


    outPutMotorAngles.x = minDisI *motorU1Max / controlSteps;
    outPutMotorAngles.y = minDisJ *motorU2Max / controlSteps;

    qDebug() <<"Current Angle: " << currentMotorAngles.x << " " << currentMotorAngles.y;
    qDebug() <<"Output Angle: " << outPutMotorAngles.x << " " << outPutMotorAngles.y;
}
