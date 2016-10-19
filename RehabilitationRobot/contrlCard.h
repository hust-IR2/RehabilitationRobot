#pragma once
// Adlink head file////////
#include "APS168.h"
#include "ErrorCodeDef.h"
#include "APS_define.h"
#include "type_def.h"
//////////////////////////
#include<QDebug>
#define YES (1)
#define NO  (0)
#define ON  (1)
#define OFF (0)
#define __MAX_DO_CH (24)
#define __MAX_DI_CH (24)
#define shoudlerAxisId 0
#define elbowAxisId 1
#define Unit_Convert  0.009  // 360/40000 传动系数，60-减数比，16384-一圈脉冲数，2pi-圆周   //
class contrlCard
{
public:
	contrlCard();
	~contrlCard();
	
	BOOL IsCardInitial(void);//检查运动控制卡是否初始化成功，成功返回true,失败返回false
	int Initial();//运动控制卡初始化函数
    void Robot_Pos(bool Optoelc_Switch[2][2]);//获取四个光电开关状态
    void SetClutch(bool onOroff=TRUE);
    void ServeTheMotor(bool onOroff=TRUE );//电机开闭，ON-开，OFF-关闭
    void SetParamZero();//使得各电机参数归零
private:

	void FunctionResult(long Ret);//输出运动控制卡错误原因
private:
	long v_card_name;
	long v_board_id;
	long v_channel;
	int v_total_axis;
	bool v_is_card_initialed;
	long Is_thread_creat;
};

