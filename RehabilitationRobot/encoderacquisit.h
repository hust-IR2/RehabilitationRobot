#ifndef ENCODERACQUISIT_H
#define ENCODERACQUISIT_H

#include <QObject>
#include <QThread>
class encoderacquisit:public QThread
{
public:
    encoderacquisit();
    ~encoderacquisit();
    void run();
    double* getAngle();
private:
    double angle[2];
    bool m_stop;
     QMutex mutex;//加数据的读写锁
    void getEncoderData(double EncoderData[2]);
};

#endif // ENCODERACQUISIT_H
