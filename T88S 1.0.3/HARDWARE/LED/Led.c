#include "Led.h"

/************************************************************
 * ��������Uart_SendData
 * ����  �����ڷ�������
 * ����  ����
 * ���  ����
 * ����  ��
 ************************************************************/
void Uart_SendData(USART_TypeDef* USARTx,uint16_t Data)
{
	  USART_SendData(USARTx, Data);
   while (!(USARTx->SR & USART_FLAG_TXE));
}

//LEDȫ��Ϩ��
void AllLedOff(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
}

/************************************************************
 * ��������Led_Init
 * ����  ��ĸ������Led4(R:PB5,G:PB4)��Led5(R:PB3,PA15)
											���ϣ�Led2(R:PB5,G:PB4)��Led8(R:PB14,GPB15)
	          ��β��Led1(R:PB9,G:PB8)��Led6(R:PB9,G:PB8)
           ���ϣ�Led3(G:PB3,G:PA15)��Led9(R:PB14,G:PB15)
           Ƭѡ��PA11,PA12
 * ����  ����
 * ���  ����
 * ����  ��main
 ************************************************************/
void Led_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
 AllLedOff();
}



/************************************************************
 * ��������Uart2_Init
 * ����  ������2���� ��TX:PA2��RX:PA3��������19200
	          �ֳ�9λ��һλֹͣλ��һλ��У��λ
											����������λ�����
 * ����  ��bound:19200
 * ���  ����
 * ����  ��TM1652_Init
 ************************************************************/
//��λ��������ݽӿ�
void Uart2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	USART_DeInit(USART2);
	//USART2_TX 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART2_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART2 ��������
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Odd;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART2,&USART_InitStructure);
	USART_Cmd(USART2,ENABLE);
}

/************************************************************
 * ��������Uart3_Init
 * ����  ������3���� ��TX:PB10��RX:PB11,������19200��
								   �ֳ�9λ��һ��ֹͣλ��һλ��У��λ
											����һ����λ�����
 * ����  ��bound :19200
 * ���  ����
 * ����  ��TM1652_Init
 ************************************************************/
void Uart3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
 USART_DeInit(USART3);
	
	//USART3_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//USART3_RX 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	//USART3 ��������
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Odd;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART3,&USART_InitStructure);
	USART_Cmd(USART3,ENABLE);	
}

/************************************************************
 * ��������TM1652_Init
 * ����  ������USART3������һ����λ�����
           ����USART2������������λ�����
 * ����  ��bound ���ڲ�����
 * ���  ����
 * ����  ����main����
 ************************************************************/
void TM1652_Init(u32 bound)
{
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);

Uart2_Init(bound);
Uart3_Init(bound);
NixieTube_AllOff();
}

//TM1652 ����ͨ��
//λ��ַ�����Ϊ6λ��
//GR1:0X08,GR2:0X88,GR3:0X48,GR4:0XC8,GR5:0X28,GR6:0XA8
//��ʾ����7�Σ�0~9��
//0:0x3F, 1:0x06, 2:0x5B, 3:0x4F, 4:0x66, 5:0x6D, 6:0x7D
//7:0x07, 8:0x7F, 9:0x6F
//
//������ʾ���ݣ�0-9
//
void SendDataNum(USART_TypeDef* USARTx,char Num)
{
	switch(Num)//��ʾ������
	{
			case 0:
			    	Uart_SendData(USARTx,0x3F);
								break;
			
			case 1:
								Uart_SendData(USARTx,0x06);
								break;
			
			case 2:
				    Uart_SendData(USARTx,0x5B);
								break;
			
			case 3:
				    Uart_SendData(USARTx,0x4F);
								break;
			
			case 4:
					   Uart_SendData(USARTx,0x66);
								break;
			
			case 5:
					   Uart_SendData(USARTx,0x6D);
								break;
			
			case 6:
					   Uart_SendData(USARTx,0x7D);
								break;
			
			case 7:
					   Uart_SendData(USARTx,0x07);
								break;
					
			case 8:
					   Uart_SendData(USARTx,0x7F);
								break;
			
			case 9:
					   Uart_SendData(USARTx,0x6F);
								break;
			case 10:
								Uart_SendData(USARTx,0x00);
								break;
			default:
								break;
		}
}

/************************************************************
 * ��������NixieTubeLowLight
 * ����  ���������ʾ�䰵
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void NixieTubeLowLight(void)
{
 u32 DisValue;	
	NixieTube_AllOff();
	if(netState.Net_Insert[0]==1|netState.Net_Insert[2]==1)
	{
		DisValue = (netparam.dis_twoship*1000)+netparam.dis_twonet;
		NixieTub_Three(DisValue);
		if(netState.Net_Insert[1]==1)
		{
			NixieTub_four(netparam.tail_z);
		}
		else
		{
			if(netState.Net_Insert[2]==1)
			{
				NixieTub_four(netparam.right_z);
			}
			else if(netState.Net_Insert[0]==1)
			{
				NixieTub_four(netparam.left_z);
			}
		}
	}
	else if(netState.Net_Insert[1]==1)
	{
		NixieTub_twoship(netparam.dis_twoship);
		NixieTub_four(netparam.tail_z);
	}
}

/************************************************************
 * ��������NixieTub_Three
 * ����  ��������λ�������ʾ��
 * ����  ��Num:��ʾ����(0~9)
 * ���  ����
 * ����  ����
 ************************************************************/
//��λ�������ʾ
void NixieTub_Three(u32 Num)
{
	char twoship_units,twoship_tens,twoship_hundreds; //twoship
	char twonet_units,twonet_tens,twonet_hundreds; //twonet
	
	twoship_units = (Num/1000)%10; //��
	twoship_tens = (Num/10000)%10; //ʮ
	twoship_hundreds = Num/100000; //��
	
	twonet_units = Num%10;
	twonet_tens = (Num/10)%10;
	twonet_hundreds = (Num/100)%10;
	
	Uart_SendData(USART2,0x08);
	SendDataNum(USART2,twoship_hundreds);
	SendDataNum(USART2,twoship_tens);
	SendDataNum(USART2,twoship_units);
	SendDataNum(USART2,twonet_hundreds);
	SendDataNum(USART2,twonet_tens);
	SendDataNum(USART2,twonet_units);
	delay_ms(6);
	Uart_SendData(USART2,0x18);//��ʾ����
	if(State == 1)//д��״̬
		Uart_SendData(USART2,0xFF);//����
	else //��д��״̬
		Uart_SendData(USART2,0xF9);//����
}


/************************************************************
 * ��������NixieTub_twoship
 * ����  ���������
 * ����  ��Num:��ʾ����(0~9)
 * ���  ����
 * ����  ����
 ************************************************************/
//���������
void NixieTub_twoship(u16 Num)
{
	char units,tens,hundreds;
	units = Num%10;
	tens = (Num/10)%10;
	hundreds = Num/100;
	
	Uart_SendData(USART2,0x08);
	SendDataNum(USART2,hundreds);
	SendDataNum(USART2,tens);
	SendDataNum(USART2,units);
	SendDataNum(USART2,10);
	SendDataNum(USART2,10);
	SendDataNum(USART2,10);
	delay_ms(6);
	Uart_SendData(USART2,0x18);//��ʾ����
	if(State == 1)//д��״̬
		Uart_SendData(USART2,0xFF);//����
	else //��д��״̬
		Uart_SendData(USART2,0xF9);//����
}

/************************************************************
 * ��������NixieTub_four
 * ����  ����λ�������ʾ
 * ����  ��Num:��ʾ����(0~9)
 * ���  ����
 * ����  ��
 ************************************************************/
void NixieTub_four(u16 Num)
{
	char units,tens,hundreds,thousands;
	units = Num%10;
	tens = (Num/10)%10;
	hundreds = (Num/100)%10;
	thousands = Num/1000;
	
	Uart_SendData(USART3,0x08);
 SendDataNum(USART3,thousands);
	SendDataNum(USART3,hundreds);
	SendDataNum(USART3,tens);
	SendDataNum(USART3,units);
	delay_ms(6);
	Uart_SendData(USART3,0x18);//��ʾ����
	if(State == 1)//д��״̬
		Uart_SendData(USART3,0xFF);//����
	else //��д��״̬
		Uart_SendData(USART3,0xF9);//����
		
}

/************************************************************
 * ��������NixieTube_AllOff
 * ����  �������ȫ������ʾ
 * ����  ����
 * ���  ����
 * ����  ����
 ************************************************************/
void NixieTube_AllOff(void)
{
	Uart_SendData(USART2,0x18);//��ʾ����
	Uart_SendData(USART2,0x00);//
	
	Uart_SendData(USART3,0x18);//��ʾ����
	Uart_SendData(USART3,0x00);//
	
	delay_ms(5);
}

//�������ʾ
void NixieTubeDisplay()
{
	if(State)//����д��״̬
	{
		NixieTube_AllOff();
		switch(netState.Net_Connet)
		{
			case 1://����
								if(netState.Net_Insert[0]==0) //���ϱ��γ�
								{
									State = 0;
									netState.Net_Connet=0;
									DisableEncode();
									if(netState.Net_Insert[2]==1)
									{
										NixieTub_Three((netparam.dis_twoship*1000+netparam.dis_twonet));
										if(netState.Net_Insert[1]==1)
											NixieTub_four(netparam.tail_z);
										else
											NixieTub_four(netparam.right_z);
									}
									else if(netState.Net_Insert[1]==1)
									{
										NixieTub_twoship(netparam.dis_twoship);
										NixieTub_four(netparam.tail_z);
									}
								}
								else if(netState.Net_Insert[0] == 1) //��λ���Ѳ���
								{
									NixieTub_Three((netparam.dis_twoship*1000+netparam.dis_twonet));
									NixieTub_four(netparam.left_z);
									
									//���ñ�����
									Encode1_Enable(netparam.dis_twoship);
									Encode2_Enable(netparam.left_z);
									Encode3_Enable(netparam.dis_twonet);
								}
								break;
			
			case 2://��β
								if(netState.Net_Insert[1]==0)//��ǰд�����λ�Ǳ��γ�
								{
									State = 0;
									netState.Net_Connet=0;
									DisableEncode();
									if(netState.Net_Insert[2]==1)
									{
										NixieTub_Three(netparam.dis_twoship*1000+netparam.dis_twonet);
										NixieTub_four(netparam.right_z);
									}
									else if(netState.Net_Insert[0]==1)
									{
										NixieTub_Three(netparam.dis_twoship*1000+netparam.dis_twonet);
										NixieTub_four(netparam.left_z);
									}
								}
								else if(netState.Net_Insert[1]==1)//��λ���Ѳ���
								{
									NixieTub_twoship(netparam.dis_twoship);
									NixieTub_four(netparam.tail_z);
									
									//���ñ�����
								 Encode2_Enable(netparam.tail_z);
									Encode1_Enable(netparam.dis_twoship);
								}
								break;
			
			case 3://����
								if(netState.Net_Insert[2]==0)//��ǰд�����λ�Ǳ��γ�
								{
									State = 0;
									netState.Net_Connet=0;
									DisableEncode();
									if(netState.Net_Insert[0]==1)//������λ���Ѳ���
									{
										NixieTub_Three(netparam.dis_twoship*1000+netparam.dis_twonet);
										if(netState.Net_Insert[1]==1)
										{
											NixieTub_four(netparam.tail_z);
										}
										else
										{
											NixieTub_four(netparam.left_z);
										}
									}
									else if(netState.Net_Insert[1]==1)//��β��λ���Ѳ���
									{
										NixieTub_twoship(netparam.dis_twoship);
										NixieTub_four(netparam.tail_z);
									}
								}
								else if(netState.Net_Insert[2]==1)//��λ���Ѳ���
								{
									//�������ʾ
									NixieTub_four(netparam.right_z);
									NixieTub_Three(netparam.dis_twoship*1000+netparam.dis_twonet);
									
									//���ñ�����
									Encode1_Enable(netparam.dis_twoship);
									Encode2_Enable(netparam.right_z);
									Encode3_Enable(netparam.dis_twonet);
								}
								break;
			default:
								break;
		}
	}
	else //��д��״̬�� //
	{
		NixieTubeLowLight();
	}
}

//���µ�״̬
void updateLedState()
{
	AllLedOff();
	
	if(State==0) //��д��״̬
	{
		LedState();
	}
	else //д��״̬
	{
		LedState();
		switch(netState.Net_Connet)
		{
			case 1:
								Led_NetLeft_GreenOff;
								Led_NetLeft_RedOn;
								break;
			
			case 2:
								Led_NetTail_GreenOff;
								Led_NetTail_RedOn;
								break;			
			
			case 3:
								Led_NetRight_GreenOff;
								Led_NetRight_RedOn;
								break;
		}
	}

	//ĸ��
	if(netparam.momship==1)
	{
		Led_ShipLeft_GreenOn;
	}
	else if(netparam.momship==2)
	{
		Led_ShipRight_GreenOn;
	}	
	
}

void LedState(void)
{
	if(netState.Net_Insert[0] == 1)//���ϲ���
	{
		Led_NetLeft_GreenOn;				
		Led_NetLeft_RedOff;
	}
	else if(netState.Net_Insert[0] == 0)//���ϰγ�
	{
		Led_NetLeft_GreenOff;
		Led_NetLeft_RedOff;
	}
	
	if(netState.Net_Insert[1]==1)//��β����
	{
		Led_NetTail_GreenOn;	
		Led_NetTail_RedOff;
	}
	else if(netState.Net_Insert[1]==0) //��β�γ�
	{  
		Led_NetTail_GreenOff;
		Led_NetTail_RedOff;
	}
	
	if(netState.Net_Insert[2] == 1)//���ϲ���
	{
		Led_NetRight_GreenOn;
		Led_NetRight_RedOff;
	}
	else if(netState.Net_Insert[2] == 0)//���ϰγ�
	{
		Led_NetRight_GreenOff;
		Led_NetRight_RedOff;
	}
}

//�������ʾ����
void NixieTube_task(void *pdata)
{
	while(1)
	{
		delay_ms(50);
		if(Nixie.Display)
		{
			Nixie.Display = 0;
			NixieTubeDisplay();
			updateLedState();
		}
		
		if(Nixie.valueChange)
		{
			Nixie.valueChange = 0;
			if(netState.Net_Connet == 1 || netState.Net_Connet == 3)
			{
				NixieTub_Three(NixieTubValue[0]*1000+NixieTubValue[2]);
				NixieTub_four(NixieTubValue[1]);
			}
			else if(netState.Net_Connet == 2)
			{
				NixieTub_twoship(NixieTubValue[0]);
				NixieTub_four(NixieTubValue[1]);
			}
		}
	}
}
