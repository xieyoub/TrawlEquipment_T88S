#ifndef _ENCODE_H
#define _ENCODE_H
#include "main.h"

void Encode_Init(void);
void TIME2_Encode(void);
void TIME3_Encode(void);
void TIME4_Encode(void);
void EXTI4_IRQhandler(void);
void EXTI9_5_IRQhandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void Encode1_Enable(u16 Num);
void Encode3_Enable(u16 Num);
void Encode2_Enable(u16 Num);
void DisableEncode(void);

#endif
