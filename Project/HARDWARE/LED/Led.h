#ifndef _LED_H
#define _LED_H
#include "main.h"

void Uart_SendData(USART_TypeDef* USARTx,uint16_t Data);
void AllLedOff(void);
void Led_Init(void);
void Uart2_Init(u32 bound);
void Uart3_Init(u32 bound);
void TM1652_Init(u32 bound);
void SendDataNum(USART_TypeDef* USARTx,char Num);
void NixieTubeLowLight(void);
void NixieTub_Three(u32 Num);
void NixieTub_twoship(u16 Num);
void RightOffsetDisplay(u16 Num);
void NixieTub_four(u16 Num);
void NixieTube_LeftRightOff(void);
void NixieTube_AllOff(void);
void NixieTubeDisplay(void);
void updateLedState(void);
void LedState(void);

 /* Ä¸¸¨´¬£ºLed4(R:PB14,G:PB15)¡¢Led5(R:PB14,G:PB15)
				×óÏÏ£ºLed2(R:PB5,G:PB4)¡¢Led8(R:PB5,G:PB4)
	   ÍøÎ²£ºLed1(R:PB9,G:PB8)¡¢Led6(R:PB9,G:PB8)
    ÓÒÏÏ£ºLed3(R:PB3,G:PA15)¡¢Led9(R:PB3,G:PA15)
    Æ¬Ñ¡£ºPA11(LED4¡¢5¡¢6¡¢8),PA12(LED1¡¢2¡¢3¡¢9)
*/
											
#define Led_NetTail_GreenOn     GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_ResetBits(GPIOB,GPIO_Pin_8);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)
																																
#define Led_NetTail_RedOn       GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_ResetBits(GPIOB,GPIO_Pin_9);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define Led_NetTail_GreenOff    GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_SetBits(GPIOB,GPIO_Pin_8);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define Led_NetTail_RedOff      GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_SetBits(GPIOB,GPIO_Pin_9);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)


#define Led_NetLeft_GreenOn     GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_ResetBits(GPIOB,GPIO_Pin_4);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define Led_NetLeft_RedOn       GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_ResetBits(GPIOB,GPIO_Pin_5);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define Led_NetLeft_GreenOff    GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_SetBits(GPIOB,GPIO_Pin_4);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define Led_NetLeft_RedOff      GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_SetBits(GPIOB,GPIO_Pin_5);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#define Led_NetRight_GreenOn    GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_ResetBits(GPIOA,GPIO_Pin_15);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define Led_NetRight_RedOn      GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_ResetBits(GPIOB,GPIO_Pin_3);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define Led_NetRight_GreenOff   GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_SetBits(GPIOA,GPIO_Pin_15);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define Led_NetRight_RedOff     GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_SetBits(GPIOB,GPIO_Pin_3);\
																																GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#define Led_ShipLeft_GreenOn    GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);\
																																GPIO_SetBits(GPIOB,GPIO_Pin_15);GPIO_ResetBits(GPIOA,GPIO_Pin_12);GPIO_ResetBits(GPIOB,GPIO_Pin_15)

#define Led_ShipRight_GreenOn   GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12);\
																																GPIO_SetBits(GPIOB,GPIO_Pin_15);GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOB,GPIO_Pin_15)
																																
#endif
