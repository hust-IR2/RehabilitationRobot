# RehabilitationRobot
 This project is designed to serve  our lab's RehabilitationRobot!
## dependencies
+ OpenCV3.1  

For more information go to http://opencv.org/.
+ Qt5.7 

 For more information go to https://www.qt.io/.

+ [CVCamera QML plugin](https://github.com/hust-IR2/rehabRobot/tree/master/module/qml-cveyecamera)

 [how to use](https://github.com/hust-IR2/rehabRobot)

+ Eigen

 **Eigen** is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.For more information go to http://eigen.tuxfamily.org/.

+ APS Library

 **APS library** provides users a uniform interface to access all of **ADLINK** products which support it. It can cover many automation fields especially in machine automation.The most important component in machine automation is **motion control**. APS library was first born with motion control which co-working components such as system platform management, field bus communication function, general digital input/output, general analog input/output and various counter/timer supports are all built-in components in APS. The APS library will be an all-in-one solution in automation field of ADLINK products.For more information go to http://www.adlinktech.com/support/index.php/.

+ NI-DAQmx Library 

 **NI-DAQmx Library** provides users a uniform interface to access all of **National Instruments** data acquisition and switch devices to develop instrumentation, acquisition, and control applications.For more information go to http://sine.ni.com/psp/app/doc/p/id/psp-268/.

## How to use
### Windows
+ Set OPENCV_HOME to replace Opencv's installation directory ,and add the library to the environment of system.
+ Download and Unzip Eigen to /Src from http://eigen.tuxfamily.org/.
+ make install CVCamera QML plugin follow the above method.
+ Open /RehabilitationRobot/RehabilitationRobot.pro, qmake it and compile it,then run it if there are no errors.



