#include "config.h"

static u8 tim2flag = 0;
u8 netbuf[3][18];
NixieTub Nixie;

/************************************************************
 * 函数名：OffSetInit
 * 描述  ：参数初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void OffSetInit()
{
	u8 i;
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x31;
	
	switch (nettemp)
	{
		case 1://左舷
							{
								tx1buf[2] = netparam.left_y>>8;
								tx1buf[3] = netparam.left_y;
								if(netparam.momship==1)
								{
									if(netparam.dis_twoship>netparam.dis_twonet)
										tx1buf[4] = 1;
									else
										tx1buf[4] = 0;
								}
								else if(netparam.momship==2)
								{
									tx1buf[4] = 0;
								}
								tx1buf[5] = netparam.left_x>>8;
								tx1buf[6] = netparam.left_x;
								tx1buf[7] = 0x01;
								for(i=0;i<18;i++)
										netbuf[0][i] = tx1buf[i];
								OSQPost(msg_write,(void*)netbuf[0]);
							}
							break;
		
		case 2://网尾
							{
								tx1buf[2] = netparam.tail_y>>8;
								tx1buf[3] = netparam.tail_y;
								if(netparam.momship==1)
									tx1buf[4] = 1;
								else
									tx1buf[4] = 0;
								
								tx1buf[5] = netparam.tail_x>>8;
								tx1buf[6] = netparam.tail_x;
								tx1buf[7] = 0x02;
								for(i=0;i<18;i++)
									netbuf[1][i] = tx1buf[i];
								OSQPost(msg_write,(void*)netbuf[1]);	
							}
							break;
		
		case 3://右舷
							{
								tx1buf[2] = netparam.right_y>>8;
								tx1buf[3] = netparam.right_y;
								if(netparam.momship==1)
								{
									tx1buf[4] = 1;
								}
								else
								{
									if(netparam.dis_twoship>netparam.dis_twonet)
										tx1buf[4] = 0;
									else
										tx1buf[4] = 1;
								}
								tx1buf[5] = netparam.right_x>>8;
								tx1buf[6] = netparam.right_x;
								tx1buf[7] = 0x03;
								for(i=0;i<18;i++)
									netbuf[2][i] = tx1buf[i];
								OSQPost(msg_write,(void*)netbuf[2]);
							}
							break;
		default:
							break;
	}
}

/************************************************************
 * 函数名：OffSetWrite
 * 描述  ：注入参数
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void OffSetWrite()
{
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x31;
	tx1buf[7] = netState.Net_Connet;
	paramClct();//参数计算
	WriteFlash_param();
}


/************************************************************
 * 函数名：OpenSerial
 * 描述  ：打开串口命令
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void OpenSerial()
{
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x17;
	tx1buf[7] = netState.Net_Sel;
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * 函数名：OffSetRead
 * 描述  ：读取命令
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void OffSetRead()
{
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x32;
	tx1buf[7] = netState.Net_Sel;
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * 函数名：CloseSerial
 * 描述  ：关闭串口命令
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void CloseSerial()
{
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x18;
 tx1buf[7] = netState.Net_Connet;
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * 函数名：HandShake
 * 描述  ：握手命令
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void HandShake(void)
{
	u8 i;
	tx1buf[0] = 0x24;
	tx1buf[1] = 0x58;
	for(i=2;i<18;i++)
		tx1buf[i] = 0;
	OSQPost(q_msg,(void*)tx1buf);
}

/************************************************************
 * 函数名：msg_crc
 * 描述  ：CRC校验
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
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

/************************************************************
 * 函数名：Led_exception
 * 描述  ：故障闪灯
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void Led_exception(void)
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


/************************************************************
 * 函数名：Postfault
 * 描述  ：故障网位仪发送
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void Postfault()
{
	switch (tx1buf[7])
	{
		case 1:
							faultCnt[0]++;
							if(faultCnt[0]>=4)
							{
								faultCnt[0] = 0;
								netState.Net_Insert[0] = 2; //左舷网标故障
								Nixie.Display = 1;
							}
							break;
		case 2:
							faultCnt[1]++;
							if(faultCnt[1]>=4)
							{
								faultCnt[1] = 0;
								netState.Net_Insert[1] = 2; //网尾网标故障
								Nixie.Display = 1;
							}		
							break;
		case 3:
							faultCnt[2]++;
							if(faultCnt[2]>=4)
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

/************************************************************
 * 函数名：Pendfault
 * 描述  ：故障网位移请求
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void Pendfault()
{
	switch (tx1buf[7])
	{
		case 1:
			    if(netState.Net_Insert[0]==2)
							{
								netState.Net_Insert[0] = 1;
								Nixie.Display = 1;
							}
							faultCnt[0]=0;
							break;
		case 2:
							if(netState.Net_Insert[1]==2)
							{
								netState.Net_Insert[1] = 1;
								Nixie.Display = 1;
							}
							faultCnt[1]=0;	
							break;
		case 3:
							if(netState.Net_Insert[2]==2)
							{
								netState.Net_Insert[2] = 1;
								Nixie.Display = 1;
							}
							faultCnt[2]=0;
							break;
		default:
							break;
	}
}

/************************************************************
 * 函数名：tmr1_callback
 * 描述  ：定时器1回调
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void tmr1_callback(OS_TMR *ptmr,void *p_arg)
{
	SilentTime++;
	if(SilentTime>300 && State==1)
	{
		State = 0; //退出写码状态
		DisableEncode();
		netState.Net_Connet = 0;
		Nixie.Display=1;
	}
}

/************************************************************
 * 函数名：tmr2_callback
 * 描述  ：定时器2回调
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void tmr2_callback(OS_TMR *ptmr,void *p_arg)
{
	Led_exception();
}

/************************************************************
 * 函数名：handshake_task
 * 描述  ：握手任务
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void handshake_task(void *pdata)
{
	u8 *rxbuf;
	INT8U err;
	while(1)
	{
		OSTimeDlyHMSM(0, 0,1,0);
		if(Usart_flag)
		{
			HandShake();
			rxbuf = (u8*)OSMboxPend(msg_hand,100,&err);
			
			if(0x51 == rxbuf[1])//收到回复
			{
				UsartAskCnt = 0;
			}
			else //未收到回复
			{
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
