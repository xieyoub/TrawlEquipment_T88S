#include "updataParam.h"


/***********************************************************
 * 函数名: paramClct
 * 描述  ：参数计算
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void paramClct()
{
	u8 i;
	int tep_x;
	tep_x = abs((netparam.dis_twoship/2) - (netparam.dis_twonet/2));
	netparam.dis_twoship = NixieTubValue[0];
	netparam.dis_twonet = NixieTubValue[2];
	switch (netState.Net_Connet)
	{
		case 1:
							{
								netparam.left_z = NixieTubValue[1];
								if(netparam.momship==1)
								{
									netparam.left_x = tep_x;
									netparam.left_y = sqrt((double)(netparam.left_z*netparam.left_z)-(tep_x*tep_x));
									if(netparam.dis_twoship>netparam.dis_twonet)
									{
										tx1buf[4]=1;
									}
									else
									{										
										tx1buf[4] = 0;
									}
								}
								else if(netparam.momship==2)
								{
									if(netparam.dis_twoship>netparam.dis_twonet)
										netparam.left_x = tep_x+netparam.dis_twonet;
									else
										netparam.left_x = netparam.dis_twonet-tep_x;
									netparam.left_y = sqrt((double)(netparam.left_z*netparam.left_z)-(tep_x*tep_x));
									tx1buf[4] = 0;
								}
								tx1buf[2] = netparam.left_y<<8;
								tx1buf[3] = netparam.left_y;
								tx1buf[5] = netparam.left_x<<8;
								tx1buf[6] = netparam.left_x;
								for(i=0;i<18;i++)
								WriteLeftBuf[i] = tx1buf[i];
								OSQPost(q_msg,(void*)WriteLeftBuf);
							}
							break;
		
		case 2:
							{
								netparam.tail_z = NixieTubValue[1];
								netparam.tail_x = netparam.dis_twoship/2;
								netparam.tail_y = sqrt((double)(netparam.tail_z*netparam.tail_z)-(netparam.tail_x*netparam.tail_x));
								tx1buf[2] = netparam.tail_y<<8;
								tx1buf[3] = netparam.tail_y;
								tx1buf[5] = netparam.tail_x<<8;
								tx1buf[6] = netparam.tail_x;
								if(netparam.momship==1)
									tx1buf[4] = 1;
								else 
									tx1buf[4] = 0;
								
								for(i=0;i<18;i++)
							 	WriteTailBuf[i] = tx1buf[i];
								OSQPost(q_msg,(void*)WriteTailBuf);								
							}
							break;
		
		case 3:
							{
								netparam.right_z = NixieTubValue[1];
								if(netparam.momship==1)
								{
									if(netparam.dis_twoship>netparam.dis_twonet)
										netparam.right_x = tep_x+netparam.dis_twonet;
									else
										netparam.right_x = netparam.dis_twonet-tep_x;
									netparam.right_y = sqrt((double)(netparam.right_z*netparam.right_z)-(tep_x*tep_x));
									tx1buf[4] = 1;
								}
								else if(netparam.momship==2)
								{
									netparam.right_x = tep_x;
									netparam.right_y = sqrt((double)(netparam.right_z*netparam.right_z)-(tep_x*tep_x));
									if(netparam.dis_twoship>netparam.dis_twonet)
										tx1buf[4] = 0;
									else
										tx1buf[4] = 1;
								}
								tx1buf[2] = netparam.right_y<<8;
								tx1buf[3] = netparam.right_y;
								tx1buf[5] = netparam.right_x<<8;
								tx1buf[6] = netparam.right_x;
								for(i=0;i<18;i++)
							 	WriteRightBuf[i] = tx1buf[i];
								OSQPost(q_msg,(void*)WriteRightBuf);			
							}
							break;
		default:
							break;
	}
}
/***********************************************************
 * 函数名: paramUpdata
 * 描述  ：参数修正
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void paramUpdata()
{ 
	u16 tep_x;
	tep_x = abs(netparam.dis_twoship/2-netparam.dis_twonet/2);
	if(netparam.momship==1)
	{
		netparam.left_x = tep_x;
		netparam.left_y = sqrt((double)(netparam.left_z*netparam.left_z)-(tep_x*tep_x));
		netparam.tail_x = netparam.dis_twoship/2;
		netparam.tail_y = sqrt((double)(netparam.tail_z*netparam.tail_z)-(netparam.tail_x*netparam.tail_x));
		if(netparam.dis_twoship>netparam.dis_twonet)
			netparam.right_x = tep_x+netparam.dis_twonet;
		else
			netparam.right_x = netparam.dis_twonet-tep_x;
		netparam.right_y = sqrt((double)(netparam.right_z*netparam.right_z)-(tep_x*tep_x));
	}
	else if(netparam.momship==2)
	{
		if(netparam.dis_twoship>netparam.dis_twonet)
			netparam.left_x = tep_x+netparam.dis_twonet;
		else
			netparam.left_x = netparam.dis_twonet-tep_x;
		
		netparam.left_y = sqrt((double)(netparam.left_z*netparam.left_z)-(tep_x*tep_x));
		netparam.tail_x = netparam.dis_twoship/2;
		netparam.tail_y = sqrt((double)(netparam.tail_z*netparam.tail_z)-(netparam.tail_x*netparam.tail_x));
		netparam.right_x = tep_x;
		netparam.right_y = sqrt((double)(netparam.right_z*netparam.right_z)-(tep_x*tep_x));
	}
	WriteFlash_param();	
}

