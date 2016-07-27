#include "detecting.h"

/***********************************************************
 * ������: NetDetecting
 * ����  �����T800�����γ�����ʾ��Ӧ�ĵƵ�״̬
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void NetDetecting()
{
 switch(NetSta[0])//����״̬
	{
		case 0://δ����
							if(netState.Net_Insert[0]!=0)
							{
								netState.Net_Insert[0] = 0;
								Nixie.Display = 1;
							}
							break;
		
		case 1://����
							if(netState.Net_Insert[0]==0)
							{
								netState.Net_Insert[0] = 1;
								nettemp = 1;
								OffSetInit();
							}
							break;
		
		case 2://����
							if(netState.Net_Insert[0]!=2)
							{
								netState.Net_Insert[0] = 2;
								Nixie.Display = 1;
							}
							break;
		default:
							break;
	}
	
	switch (NetSta[1])//��β
	{
		case 0://δ����
							if(netState.Net_Insert[1]!=0)
							{
								netState.Net_Insert[1] = 0;
								Nixie.Display = 1;
							}
							break;
		
		case 1://����
							if(netState.Net_Insert[1]==0)
							{
								netState.Net_Insert[1] = 1;
								nettemp = 2;
								OffSetInit();
							}
							break;
		
		case 2://����
							if(netState.Net_Insert[1]!=2)
							{
								netState.Net_Insert[1] = 2;
								Nixie.Display = 1;
							}
							break;
		default:
							break;		
	}
	
	switch(NetSta[2])//����
	{
		case 0://δ����
							if(netState.Net_Insert[2]!=0)
							{
								netState.Net_Insert[2] = 0;
								Nixie.Display = 1;
							}
							break;
		
		case 1://����
							if(netState.Net_Insert[2]==0)
							{
								netState.Net_Insert[2] = 1;
								nettemp = 3;
								OffSetInit();
							}
							break;
		
		case 2://����
							if(netState.Net_Insert[2]!=2)
							{
								netState.Net_Insert[2] = 2;
								Nixie.Display = 1;
							}
							break;
		default:
							break;	
	}	
}

/***********************************************************
 * ������: paramSet_task
 * ����  ����һ�β���ʱ��ʼ����������
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void paramSet_task(void *pdata)
{
	u8 *rxbuf;
	u8 *txbuf;
	u8 numbering; 
	u8 err,i;
	while(1)
	{
		OSTimeDlyHMSM(0, 0,0,100);
		txbuf = OSQPend(msg_write,0,&err);
		for(i=0;i<18;i++)
			tx1buf[i] = txbuf[i];
		SendCnt = 0;
		if(err==OS_ERR_NONE)
		{
			//��������
			OSTaskSuspend(KEY_TASK_PRIO);
			OSTaskSuspend(USARTSEND_TASK_PRIO);
			OSTaskSuspend(HANDSHAKE_TASK_PRIO);
			
			numbering = tx1buf[7];
			Com1SendData();
			SendCnt++;
			while(SendCnt!=0)
			{
				rxbuf = OSMboxPend(msg_receive,200,&err);
				if(err==OS_ERR_NONE)
				{
					if(rxbuf[1]==0x31)//ע��ɹ�
					{
						SendCnt = 0;
						err=OSQQuery(msg_write,&q_data);//��ѯ�������Ƿ�����Ϣ
						if(err==OS_ERR_NONE)
						{
							if(q_data.OSNMsgs==0)
							{
								if(State==1)
								{
									State = 0; //�˳�д��״̬
									DisableEncode();
									netState.Net_Connet = 0;
								}
								Nixie.Display = 1;
							}
						}
						switch(numbering)
						{
							case 1:
												netState.Net_Insert[0] = 1;
												break;
							
							case 2:
												netState.Net_Insert[1] = 1;
												break;
							
							case 3:
												netState.Net_Insert[2] = 1;
												break;
							default:
												break;
						}
					}
				}
				else if(err==OS_ERR_TIMEOUT)//��ʱ
				{
					SendCnt++;
					if(SendCnt<4)//���ʹ���δ����3��
					{
						Com1SendData();
					}
					else //���ʹ�����3�� 
					{
						SendCnt = 0;
						switch(numbering)
						{
							case 1:
												netState.Net_Insert[0] = 2;
												break;
							
							case 2:
												netState.Net_Insert[1] = 2;
												break;
							
							case 3:
												netState.Net_Insert[2] = 2;
												break;
						}
					}
				}
			}
			OSTaskResume(KEY_TASK_PRIO);
			OSTaskResume(USARTSEND_TASK_PRIO);
			OSTaskResume(HANDSHAKE_TASK_PRIO);
		}
	}
}
