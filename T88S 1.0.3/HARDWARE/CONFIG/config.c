#include "config.h"

static u8 tim2flag = 0;
u8 netbuf[3][18];
NixieTub Nixie;

//串口等待计时，超时5s
void Time1_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 24999;   //500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 1439;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM1,ENABLE);
}

//串口等待超时处理
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)
	{		
		if(UsartUsing==0)
		{
			UsartUsingTime++;
			if(UsartUsingTime>4)
			{
				UsartUsingTime = 0;
				UsartUsing=1;
				
				//超过三次串口都没回，判断对于的该网标为故障
				faultJudge();
			}
		}
		else
		{
			UsartUsingTime=0;
		}
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}

void OffSetInit()
{
	u8 i;
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x31;
	if(netparam.momship==1)
		tx1buf[4] = 1;
	else if(netparam.momship==2)
		tx1buf[4] = 0;
	
	switch (nettemp)
	{
		case 1://左舷
							{
								tx1buf[2] = netparam.left_y>>8;
								tx1buf[3] = netparam.left_y;
								tx1buf[5] = netparam.left_x>>8;
								tx1buf[6] = netparam.left_x;
								tx1buf[7] = 0x01;
								tx1buf[8] = 1; //第一次注入
								for(i=0;i<18;i++)
									netbuf[0][i] = tx1buf[i];
								OSQPost(msg_write,(void*)netbuf[0]);
							}
							break;
		
		case 2://网尾
							{
								tx1buf[2] = netparam.tail_y>>8;
								tx1buf[3] = netparam.tail_y;
								tx1buf[5] = netparam.tail_x>>8;
								tx1buf[6] = netparam.tail_x;
								tx1buf[7] = 0x02;
								tx1buf[8] = 1; //第一次注入
								for(i=0;i<18;i++)
									netbuf[1][i] = tx1buf[i];
								OSQPost(msg_write,(void*)netbuf[1]);	
							}
							break;
		
		case 3://右舷
							{
								tx1buf[2] = netparam.right_y>>8;
								tx1buf[3] = netparam.right_y;
								tx1buf[5] = netparam.right_x>>8;
								tx1buf[6] = netparam.right_x;
								tx1buf[7] = 0x03;
								tx1buf[8] = 1; //第一次注入
								for(i=0;i<18;i++)
									netbuf[2][i] = tx1buf[i];
								OSQPost(msg_write,(void*)netbuf[2]);
							}
							break;
		default:
							break;
	}
	//OSQPost(msg_write,(void*)netbuf[2]);
}

//注入偏置距离
void OffSetWrite()
{
	u8 i;
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x31;
	//updateOffset();
	paramClct();
	
	if(netparam.momship == 1)
		tx1buf[4] = 1;
	else if(netparam.momship == 2)
		tx1buf[4] = 0;
	
	tx1buf[7] = netState.Net_Connet;
	for(i=8;i<18;i++)
	 tx1buf[i] = 0;
	OSQPost(q_msg,(void*)tx1buf);
 //Com1SendData();
}


//打开串口命令，进入写码状态
void OpenSerial()
{
	//UsartUsing = 0; //串口等待状态
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x17;
	tx1buf[7] = netState.Net_Sel;
	OSQPost(q_msg,(void*)tx1buf);
	//Com1SendData();
}

//读取
void OffSetRead()
{
	//UsartUsing = 0; //串口等待状态
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x32;
	tx1buf[7] = netState.Net_Sel;
	OSQPost(q_msg,(void*)tx1buf);
	//Com1SendData();
}

//关闭串口，退出写码状态
void CloseSerial()
{
	//UsartUsing = 0; //串口等待状态
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x18;
 tx1buf[7] = netState.Net_Connet;
	OSQPost(q_msg,(void*)tx1buf);
	//Com1SendData();
}

//定时发送串口给T81
void AskConnect(void)
{
	//UsartUsing = 0;
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x58;
	OSQPost(q_msg,(void*)tx1buf);
	//Com1SendData();
	//UsartUsing = 1;
}

//CRC 校验
u16 msg_crc(u8 *ptr,u8 num)
{
	u16 crc=0xffff;
	u8 i;
	u16 gx=0x1021;
	while(num--)
	{
		for(i=0x01;i!=0;i<<=1)
		{
			if((crc&0x8000)!=0)
			{
				crc<<=1;
				crc^=gx;
			}
			else
			{
				crc<<=1;
			}
			if(((*ptr)&i)!=0)
			{
				crc^=gx;
			}
		}
		ptr++;
	}
	return ~crc;
}

void exception(void)
{
		tim2flag = !tim2flag;
		
		if(tim2flag)
		{
			if(netState.Net_Insert[0] == 2)
			{
				Led_NetLeft_RedOn;
			}
			if(netState.Net_Insert[1] == 2)
			{
				Led_NetTail_RedOn;
			}
			if(netState.Net_Insert[2] == 2)
			{
				Led_NetRight_RedOn;
			}
		}
		else
		{
			if(netState.Net_Insert[0] == 2)
			{
				Led_NetLeft_RedOff;
			}
			if(netState.Net_Insert[1] == 2)
			{
				Led_NetTail_RedOff;
			}
			if(netState.Net_Insert[2] == 2)
			{
				Led_NetRight_RedOff;
			}
		}	
}

//故障网标判断
void faultJudge()
{
	switch (netState.Net_Sel)
	{
		case 1:
							faultCnt[0]++;
							if(faultCnt[0]>=3)
							{
								faultCnt[0] = 0;
								netState.Net_Insert[0] = 2; //左舷网标故障
								Nixie.Display = 1;
							}
							break;
		case 2:
							faultCnt[1]++;
							if(faultCnt[1]>=3)
							{
								faultCnt[1] = 0;
								netState.Net_Insert[1] = 2; //网尾网标故障
								Nixie.Display = 1;
							}		
							break;
		case 3:
							faultCnt[2]++;
							if(faultCnt[2]>=3)
							{
								faultCnt[2] = 0;
								netState.Net_Insert[2] = 2; //右舷网标故障
								Nixie.Display = 1;
							}		
							break;
		default:
							break;
	}
}

//静默时间计时
void tmr1_callback(OS_TMR *ptmr,void *p_arg)
{
	SilentTime++;
	if(SilentTime>300 && State==1)
	{
		CloseSerial();
	}
}

//故障闪灯
void tmr2_callback(OS_TMR *ptmr,void *p_arg)
{
	exception();
}

//握手任务
void handshake_task(void *pdata)
{
	u8 *rxbuf;
	INT8U err;
	while(1)
	{
		OSTimeDlyHMSM(0, 0,1,0);
		if(Usart_flag)
		{
			AskConnect();
			rxbuf = (u8*)OSMboxPend(msg_hand,100,&err);
			
			if(0x51 == rxbuf[1])//收到回复
			{
				UsartAskCnt = 0;
			}
			else //未收到回复
			{
				Usart_flag = 1;
				if(UsartAskCnt<=5)
					UsartAskCnt++;
				
				if(UsartAskCnt==5)
				{
					State = 0;
					netState.Net_Insert[0] = 0;
					netState.Net_Insert[1] = 0;
					netState.Net_Insert[2] = 0;
					DisableEncode();
					AllLedOff();
					NixieTube_AllOff();
				}
			}
		}
	}
}
