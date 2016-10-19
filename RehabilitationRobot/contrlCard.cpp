#include "contrlCard.h"
contrlCard::contrlCard()
{
	v_card_name = 0;
	v_board_id = 0;
	v_channel = 0;
	v_total_axis = 0;
	v_is_card_initialed = 0;
	Is_thread_creat = 0;
}
contrlCard::~contrlCard()
{

}

void contrlCard::FunctionResult(long Ret)
{
    qDebug()<<"Initialed Error code is :"<<Ret;
}

BOOL contrlCard::IsCardInitial(void)
{
	if (v_is_card_initialed == NO)
	{
        qDebug()<<"Not initialed.";
		return NO;
	}
	else
	{
		return YES;
	}
}

int contrlCard::Initial()
{
	I32 boardID_InBits = 0;
	I32 mode = 0;
	I32 ret = 0;
	I32 i = 0;
	I32 card_name = 0;
	I32 tamp = 0;
	I32 StartAxisID = 0;
	I32 TotalAxisNum = 0;
	// Card(Board) initial
	ret = APS_initial(&boardID_InBits, mode);
	if (ret == ERR_NoError)
	{
		for (i = 0; i < 16; i++)
		{
			tamp = (boardID_InBits >> i) & 1;

			if (tamp == 1)
			{
				ret = APS_get_card_name(i, &card_name);

				if (card_name == DEVICE_NAME_PCI_8258
					|| card_name == DEVICE_NAME_AMP_82548)
				{
					ret = APS_get_first_axisId(i, &StartAxisID, &TotalAxisNum);

					//----------------------------------------------------
					v_card_name = card_name;
					v_board_id = i;
					v_total_axis = TotalAxisNum;
					v_is_card_initialed = YES;
					//电机轴参数归零
					SetParamZero();
					//从flash中load设置的参数
					APS_load_parameter_from_flash(0);

					if (v_total_axis == 4) v_channel = 2;
					else if (v_total_axis == 8) v_channel = 4;
					//----------------------------------------------------

					Is_thread_creat = NO;
					break;
				}
			}
		}

		if (v_board_id == -1)
		{
			v_is_card_initialed = NO;
            qDebug()<<"Board Id search error !";
		}
		return 1;
	}
	else
	{
		v_is_card_initialed = NO;
        qDebug()<<"Initial fail!";
		FunctionResult(ret);
		return 0;
	}
}
void contrlCard::Robot_Pos(bool Optoelc_Switch[2][2])
{
	I32 DI_Group = 0; // If DI channel less than 32
	I32 DI_Data = 0; // Di data
	I32 di_ch[__MAX_DI_CH];
	I32 returnCode = 0; // Function return code
	returnCode = APS_read_d_input(0, DI_Group, &DI_Data);
	for (int i = 0; i < __MAX_DI_CH; i++)
		di_ch[i] = ((DI_Data >> i) & 1);

    Optoelc_Switch[0][0]=di_ch[16];//0号电机ORG信号
    Optoelc_Switch[0][1]=di_ch[17];//0号电机MEL信号

    Optoelc_Switch[1][0]=di_ch[18];//1号电机ORG信号
    Optoelc_Switch[1][1]=di_ch[19];//1号电机MEL信号
}
//开关离合器 true-离合器闭合，false-离合器断开
void contrlCard::SetClutch(bool onOroff)
{
    bool do_ch[__MAX_DO_CH]={0};
    I32 digital_output_value = 0;
    I32 returnCode=0;
    do_ch[8]=onOroff;
    do_ch[9]=onOroff;
    for( int i = 0; i < __MAX_DO_CH; i++ )
            digital_output_value |= ( do_ch[i] << i );

    returnCode=APS_write_d_output( 0
            , 0                     // I32 DO_Group
            , digital_output_value  // I32 DO_Data
            );
	if (returnCode != 0)
    {
        if(onOroff)
            qDebug()<<"RelaySwitch on failed!";
        else
            qDebug()<<"RelaySwitch off failed!";
    }

}
//使得各电机参数归零
void contrlCard::SetParamZero()
{
	for (I32 Axis_ID = 0; Axis_ID < v_total_axis; Axis_ID++)
	{
		APS_set_command_f(Axis_ID, 0.0);//设置命令位置为0
		APS_set_position_f(Axis_ID, 0.0);//设置反馈位置为0
	}
}
void contrlCard::ServeTheMotor(bool onOroff )
{
    for(int i=shoudlerAxisId;i<=elbowAxisId;i++)
      {
            APS_set_axis_param_f( i, PRA_CURVE,      0.5 );//Set acceleration rate
            APS_set_axis_param_f( i, PRA_ACC,    300000.0 ); //Set acceleration rate
            APS_set_axis_param_f( i, PRA_DEC,    300000.0 ); //Set deceleration rate
            APS_set_axis_param_f( i, PRA_STP_DEC, 50000.0 );
            APS_stop_move(i);
            APS_set_servo_on(i, onOroff);
      }
}
