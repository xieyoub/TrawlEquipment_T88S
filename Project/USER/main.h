#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "delay.h"
#include "Led.h"
#include "encode.h"
#include "key.h"
#include "usart.h"
#include "config.h"
#include "sys.h"
#include "flashwr.h"
#include "math.h"
#include "detecting.h"
#include "updataParam.h"
#include "includes.h"

extern u16 NixieTubValue[3];

extern u16 SilentTime;
extern u8 State;           
extern u8 UsartUsing;
extern u8 UsartUsingTime;
extern u8 UsartAskCnt;
extern u8 faultCnt[3];
extern u8 NetSta[3];
extern u8 Usart_flag;
extern u8 SendCnt;
extern u8 nettemp;

typedef struct _NetParam NetParam;

struct _NetParam
{
	u8 momship;//ĸ�� 1����2����
	u8 NetNum; //1�����ϣ�2����β��3������
	u16 dis_twoship,dis_twonet; //������࣬���ڼ��
	u16 left_x,left_y,left_z;//���ϲ���
	u16 tail_x,tail_y,tail_z;//��β����
	u16 right_x,right_y,right_z;//���ϲ���
};
extern NetParam netparam;
extern OS_Q_DATA q_data;

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�������ȼ�
#define START_TASK_PRIO										10
//���������ջ��С
#define START_TASK_SIZE          64
//������
void start_task(void *pdata);

//��������
//�������ȼ�
#define KEY_TASK_PRIO										3
//�����ջ��С
#define KEY_STK_SIZE										64
//������
void key_task(void *pdata);

//T81��������
//�������ȼ�
#define HANDSHAKE_TASK_PRIO										8
//��ջ��С
#define HANDSHAKE_STK_SIZE										64
//������
void handshake_task(void *pdata);

//�������ʾ����
//�������ȼ�
#define NixieTube_TASK_PRIO										6
//��ջ��С
#define NixieTube_STK_SIZE										64
//������
void NixieTube_task(void *pdata);

//���ڷ�������
//�������ȼ�
#define USARTSEND_TASK_PRIO									4
//��ջ��С
#define USARTSEND_STK_SIZE									256
//������
void usartsend_task(void *pdata);

//����ʱע�������
//�������ȼ�
#define PARAMSET_TASK_PRIO          5
//��ջ��С
#define PARAMSET_STK_SIZE          256
//������
void paramSet_task(void *pdata);

//�����ʱ���ص�����
void tmr1_callback(OS_TMR *ptmr,void *p_arg);
void tmr2_callback(OS_TMR *ptmr,void *p_arg);

//�����
typedef struct _NixieTub NixieTub;

struct _NixieTub
{
 u8 Display;
	u8 valueChange;
};

extern NixieTub Nixie;

//����
typedef struct _Usartsend Usartsend;

struct _Usartsend
{
	u8 type_UsartData;
};



typedef struct _NetState NetState;

struct _NetState 
{
	u8 Net_Sel;     //ѡ�е���λ��
	u8 Net_Connet;  //��ǰд�����λ��
	u8 Net_Insert[3]; //��̨��λ�ǵ�״̬
	u8 fault;         //����
};

extern NetState netState;

extern OS_EVENT * q_msg;			//��Ϣ����
extern OS_EVENT * uart;
extern OS_EVENT * msg_hand;	
extern OS_EVENT * msg_write;
extern OS_EVENT * msg_receive;

#endif 
