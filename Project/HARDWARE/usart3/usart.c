#include "sys.h"
#include "usart.h"	  
static u8 err;

u8 rx1buf[UART_RX1_LEN];  //串口1接收缓存 0:帧头 0x24; 1: 命令(0x01:插拔, 0x31:注入, 0x32:读取);\
                            2~3:后方偏置值; 4:左右舷判断(0:左舷,1:右舷); 5~6: 左右舷偏置值;

u8 tx1buf[UART_RX1_LEN]; //串口1发送缓冲 0:帧头 0x24; 1: 命令(0x01:插拔,  0x31:注入，0x32读取);\
                      2~3:后方偏置值; 4:左右舷判断(0:左舷,1:右舷); 5~6: 左右舷偏置值;\
																						7: 选择要写码的示位标(1:网尾,2:左舷,3:右舷)
														
u8 WriteLeftBuf[18];
u8 WriteTailBuf[18];
u8 WriteRightBuf[18];
u8 ReadBuf[18];

 /***********************************************************
 * 函数名: USART1_ModeInit
 * 描述  ：波特率9600，字长8位，一位停止位，无奇偶校验位，
	          收发模式
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void USART1_ModeInit(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
}

 /***********************************************************
 * 函数名: USART1_GPIOInit
 * 描述  ：配置串口1GPIO
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void USART1_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//TX Control
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //RX Control
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //485
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	Usart1_Receive;
}

 /***********************************************************
 * 函数名: USART1_NVIC_Configuration
 * 描述  ：配置串口1中断
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void USART1_NVIC_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure; 
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

 /***********************************************************
 * 函数名: UART1_DMA_Configuration
 * 描述  ：配置串口1 DMA配置
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void UART1_DMA_Configuration(void)
{
		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //启动DMA时钟  

		DMA_DeInit(DMA1_Channel5); 	//USART3 DMA1通道3配置   
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);//外设地址  
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)rx1buf;     //内存地址  
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //dma传输方向从外设到内存 
		DMA_InitStructure.DMA_BufferSize = UART_RX1_LEN;               //设置DMA在传输时缓冲区的长度  
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设置DMA的外设地址不变 
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;      //设置DMA的内存递增模式  
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据字长 8位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;    //内存数据字长8位   
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                //设置DMA的传输模式   
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;      //设置DMA的优先级别 最高
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                 //设置DMA的2个memory中的变量互相访问  
		DMA_Init(DMA1_Channel5,&DMA_InitStructure);  

		DMA_Cmd(DMA1_Channel5,ENABLE);
}

 /***********************************************************
 * 函数名: USART1_Config
 * 描述  ：配置串口3
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void USART1_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	UART1_DMA_Configuration();
	USART1_NVIC_Configuration();
	USART1_GPIOInit();
	USART1_ModeInit();
	
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //禁止 
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);	//采用DMA方式接收 
 USART_Cmd(USART1, ENABLE);
	
}

 /***********************************************************
 * 函数名: USART1_IRQHandler
 * 描述  ：配置串口1中断处理
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void USART1_IRQHandler(void)
{   
	u32 Length = 0;
	u16 crcdata,scrData; //校验生成的数据, 源校验的数据
	
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
	{  
		DMA_Cmd(DMA1_Channel5,DISABLE); 
		Length = USART1->SR;  
		Length = USART1->DR; //清USART_IT_IDLE标志  
		
		scrData = (rx1buf[16]<<8)+rx1buf[17];
		crcdata = msg_crc(rx1buf,16); //数据校验
		
		if(crcdata == scrData)
		{
			GetCom1Data();
			if(rx1buf[1]!=0x51)
				Pendfault();
			OSSemPost(uart);
		}
		
		DMA1_Channel5->CNDTR = UART_RX1_LEN;//重装填,并让接收地址偏址从0开始
		DMA_Cmd(DMA1_Channel5, ENABLE);//处理完,重开DMA   
	}
	__nop();   	
	
}

/************************************************************
 * 函数名：WriteParam
 * 描述  ：注入参数
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void WriteParam()
{
	paramUpdata();
	if(netState.Net_Insert[0]==1)
	{
		nettemp = 1;
		OffSetInit();
	}
	if(netState.Net_Insert[1]==1)
	{
		nettemp = 2;
		OffSetInit();
	}
	if(netState.Net_Insert[2]==1)
	{
		nettemp = 3;
		OffSetInit();
	}	
}

 /***********************************************************
 * 函数名: GetCom1Data
 * 描述  ：配置串口1获取数据及处理
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void GetCom1Data(void)
{
	if(rx1buf[0]==0x24) //帧头判断
	{
		switch (rx1buf[1])
		{
			case 0x51:// 握手命令
								{
									NetSta[0] = rx1buf[14]>>6; //左舷状态
									NetSta[1] = (rx1buf[14]>>4)&0x03; //网尾状态
									NetSta[2] = (rx1buf[14]>>2)&0x03; //右舷状态
									NetDetecting();
									OSMboxPost(msg_hand,(void*)rx1buf);//发送消息
								}
								break;
			
			case 0x31: //注入成功
			    {
								if(SendCnt==0)
								{
									State = 0; //退出写码状态
									DisableEncode();
									Nixie.Display = 1;
									netState.Net_Connet = 0;
								}
								else //插上时注入参数应答
								{
									OSMboxPost(msg_receive,(void*)rx1buf);
									OSMboxPend(uart,1,&err);
								}
								
							}
				   break;
			
			default:
				   break;
		}
	}
}

/***********************************************************
 * 函数名: Com1SendData
 * 描述  ：给T81发送数据
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void Com1SendData()
{
	u8 i;
	u16 data;
	
	data = msg_crc(tx1buf,16);	//CRC校验数据生成
	tx1buf[16] = data>>8;
	tx1buf[17] = data;
	
	Usart1_OutPut;
	delay_ms(5);
	for(i=0;i<18;i++)	
	{
		USART_SendData(USART1, tx1buf[i]);
		while (!(USART1->SR & USART_FLAG_TXE));
	}
	delay_ms(5);
	Usart1_Receive;
}

/************************************************************
 * 函数名：usartsend_task
 * 描述  ：串口发送任务
 * 输入  ：无
 * 输出  ：无
 * 调用  ：无
 ************************************************************/
void usartsend_task(void *pdata)
{
	INT8U err;
	u8 *txbuf,i;
	while(1)
	{
		txbuf = OSQPend(q_msg,0,&err);
		for(i=0;i<18;i++)
			tx1buf[i] = txbuf[i];
		if(err==OS_ERR_NONE)
		{
			OSSemPend(uart,0,&err);
			if(tx1buf[1]!=0x51)
				Postfault();
			Usart_flag = 0;
			
			Com1SendData();
			
			OSSemPend(uart,200,&err);//等待1s
			if(err==OS_ERR_NONE)
			{
				Usart_flag = 1;
				OSSemPost(uart);
			}
			else if(err==OS_ERR_TIMEOUT)
			{
				Usart_flag = 1;
				OSSemPost(uart);
			}
		}

		OSTimeDlyHMSM(0, 0,0,10);
	}
}

