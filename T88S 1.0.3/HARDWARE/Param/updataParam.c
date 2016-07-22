#include "updataParam.h"

u16 x,y,z;//横偏置，纵偏置，离母船的距离

//参数计算
void paramClct()
{
	u16 twoship,twonet,tep_x;
 twoship = NixieTubValue[0];
	twonet = NixieTubValue[2];
	z = NixieTubValue[1];
	tep_x = (netparam.dis_twoship/2) - (netparam.dis_twonet/2);
	switch (netState.Net_Connet)
	{
		case 1:
							{
								if(netparam.momship==1)
								{
									x = tep_x;
									y = sqrt((double)(z*z)-(tep_x*tep_x));
								}
								else if(netparam.momship==2)
								{
									x = tep_x+twonet;
									y = sqrt((double)(z*z)-(tep_x*tep_x));
								}
								tx1buf[2] = y<<8;
								tx1buf[3] = y;
								tx1buf[5] = x<<8;
								tx1buf[6] = x;
							}
							break;
		
		case 2:
							{
								x = twoship/2;
								y = sqrt((double)(z*z)-(x*x));
								tx1buf[2] = y<<8;
								tx1buf[3] = y;
								tx1buf[5] = x<<8;
								tx1buf[6] = y;
							}
							break;
		
		case 3:
							{
								if(netparam.momship==1)
								{
									x = tep_x+twonet;
									y = sqrt((double)(z*z)-(tep_x*tep_x));
								}
								else if(netparam.momship==2)
								{
									x = tep_x;
									y = sqrt((double)(z*z)-(tep_x*tep_x));
								}
								tx1buf[2] = y<<8;
								tx1buf[3] = y;
								tx1buf[5] = x<<8;
								tx1buf[6] = x;
							}
							break;
		default:
							break;
	}
}

//参数修正
void paramUpdata()
{ 
	u16 tep_x;
	tep_x = netparam.dis_twoship/2-netparam.dis_twonet/2;
	if(netparam.momship==1)
	{
		netparam.left_x = tep_x;
		netparam.left_y = sqrt((double)(netparam.left_z*netparam.left_z)-(tep_x*tep_x));
		netparam.tail_x = netparam.dis_twoship/2;
		netparam.tail_y = sqrt((double)(netparam.tail_z*netparam.tail_z)-(netparam.tail_x*netparam.tail_x));
		netparam.right_x = tep_x+netparam.dis_twonet;
		netparam.right_y = sqrt((double)(netparam.right_z*netparam.right_z)-(tep_x*tep_x));
	}
	else if(netparam.momship==2)
	{
		netparam.left_x = tep_x+netparam.dis_twonet;
		netparam.left_y = sqrt((double)(netparam.left_z*netparam.left_z)-(tep_x*tep_x));
		netparam.tail_x = netparam.dis_twoship/2;
		netparam.tail_y = sqrt((double)(netparam.tail_z*netparam.tail_z)-(netparam.tail_x*netparam.tail_x));
		netparam.right_x = tep_x;
		netparam.right_y = sqrt((double)(netparam.right_z*netparam.right_z)-(tep_x*tep_x));
	}
	WriteFlash_param();	
}

